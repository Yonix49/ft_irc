/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:04:18 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/31 19:00:04 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
{
	_serv.clientSocket = 0;
	_serv.serverSocket = 0;
}

Server::~Server()
{
}
// Constructeur
// Server::Server()
// {
// 	_checking_nc = 0;
// }
Server::Server(int port, const std::string &password)
{
	_port = port;
	_password = password;
	_serv.clientSocket = 0;
	_serv.serverSocket = 0;
	_checking_nc = 0;
}

// Constructeur de copie
Server::Server(const Server &src)
{
	_password = src._password;
	// _vec = src._vec;
	_serv = src._serv;
	_port = src._port;
}

// Opérateur d'assignation
Server &Server::operator=(const Server &src)
{
	if (this != &src)
	{
		this->_password = src._password;
		// this->_vec = src._vec;
		this->_serv = src._serv;
		this->_port = src._port;
	}
	return *this;
}
//---------------------SET_VAR---------------------------///////

void Server::setPort(long int port)
{
	_port = port;
}

void Server::setPassword(std::string password)
{
	_password = password;
}
//---------------------GETTEUR---------------------------///////

User *Server::getUserNo(int fd)
{
	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->getFd() == fd)
			return &(*it);
	}
	return (NULL);
}

int findUser(std::string str, std::vector<User> users)
{
	int i = 0;
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (!str.compare((*it).getNickname()))
			return (i);
		i++;
	}
	return (-1);
}

User *Server::getUserString(std::string nickname)
{
	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (!it->getNickname().compare(nickname))
			return &(*it);
	}
	return (NULL);
}

std::vector<std::string> Server::get_vector_ref(std::string str)
{
	std::istringstream iss(str); // Utiliser un std::istringstream pour diviser en mots
	std::vector<std::string> words;

	std::string word;
	while (iss >> word)
		words.push_back(word);
	return (words);
}
std::string Server::get_password(void)
{
	return (_password);
}
//---------------------GETTEUR---------------------------///////
int Server::createServerSocket()
{
	int numEvents;
	struct epoll_event event;
	struct epoll_event events[10]; // Nombre maximal d'événements à gérer
	int fd;
	int isNewUser;

	_serv.epollFd = epoll_create(1);
	initializeCommandMap();
	if (_serv.epollFd == -1)
	{
		std::cerr << "Erreur lors de la création d'epoll." << std::endl;
		return 1;
	}
	launchSocket();

	event.events = EPOLLIN;
	event.data.fd = _serv.serverSocket; // Ajoutez le socket du serveur à l'instance epoll
	if (epoll_ctl(_serv.epollFd, EPOLL_CTL_ADD, _serv.serverSocket, &event) == -1)
	{
		std::cerr << "Erreur lors de l'ajout du socket du serveur à epoll." << std::endl;
		return 1;
	}
	signal(SIGINT, signal_ctrl_c);
	isNewUser = 0;
	while (!g_signal)
	{
		numEvents = epoll_wait(_serv.epollFd, events, 10, -1);
		if (numEvents == -1)
		{
			if (!g_signal)
				std::cerr << "Erreur lors de l'appel à epoll_wait." << std::endl;
			else
				std::cerr << "CNTRL+C detected (epoll_wait)." << std::endl;
			return 1;
		}
		for (int i = 0; i < numEvents; i++)
		{
			fd = events[i].data.fd;
			if (fd == _serv.serverSocket)
			{
				// Gérer une nouvelle connexion entrante
				_serv.clientSocket = accept(_serv.serverSocket, (struct sockaddr *)&_serv.clientAddress, &_serv.clientAddrLen);
				if (_serv.clientSocket == -1)
					std::cerr << "Erreur lors de l'acceptation de la connexion entrante." << std::endl;
				else
				{
					std::cout << "ON ENTRE DANS NEWUSER" << std::endl;
					newUser(_serv.clientSocket, NULL);
					isNewUser = 1;
				}
			}
			else
			{
				recieve_data(fd, 0);
				isNewUser = 0;
			}
		}
	}
	close(_serv.clientSocket);
	return (_serv.serverSocket);
}

void Server::set_Error_user(std::string error, int fd)
{
	User *user = NULL; // Déclarer un pointeur vers un utilisateur
	user = getUserNo(fd);
	user->setError(error);
}

std::string Server::get_Error_user(int fd)
{
	User *user = NULL; // Déclarer un pointeur vers un utilisateur
	user = getUserNo(fd);
	if (!user)
		return (NULL);
	return (user->Get_Error());
}
int Server::recieve_data(int fd, int isNewUser)
{
	(void)isNewUser;
	int bytesRead;
	// int		bytesSent;
	int bytesSent2;
	char buffer[1024];

	bytesRead = recv(fd, buffer, sizeof(buffer), 0);
	if (bytesRead < 0)
		std::cerr << "Erreur lors de la réception de données du client." << std::endl;
	else if (bytesRead == 0)
	{
		bytesSent2 = send(fd, "Deconnection reçu par le serveur.\n", 34, 0);
		if (bytesSent2 < 0)
			std::cerr << "Erreur lors de l'envoi de la confirmation au client." << std::endl;
		std::cout << "User disconnected" << std::endl;
		// Code pour retirer le socket client de votre liste de sockets et d'_serv.epollFd si nécessaire.
		close(fd);
	}
	buffer[bytesRead] = '\0';
	std::string str(buffer);					   // Convertir le buffer en std::string
	if (!str.empty() && is_connected(fd) == false) // Ici faut mettre un bool c'est que pour la connextion ca
	{

		if (str.length() >= 2 && is_connected(fd) == false)
		{
			std::string lastTwoChars = str.substr(str.length() - 2, 2);
			if (is_connected(fd) == false && lastTwoChars == "\r\n")
			{
				irssi_check(buffer, fd);
				if (is_connected(fd) == true)
				{
					std::string welcomeMessage = RPL_WELCOME(_users.back().getNickname());
					size_t messageLength = welcomeMessage.length();
					if (send(fd, welcomeMessage.c_str(), messageLength, 0) == -1)
						std::cout << "error envoie RPL" << std::endl;
					for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
					{
						std::cout << "========================== nick = " << it->getNickname() << std::endl;
						std::cout << "========================== fd = " << it->getFd() << std::endl;
						std::cout << "========================== is_connected = " << it->get_check_in_server() << std::endl
								<< std::endl;
					}
				}
			}
			else
			{
				nc_check(buffer, fd);
				if (is_connected(fd) == true)
				{
					std::string welcomeMessage = RPL_WELCOME(_users.back().getNickname());
					size_t messageLength = welcomeMessage.length();
					if (send(fd, welcomeMessage.c_str(), messageLength, 0) == -1)
						std::cout << "error envoie RPL" << std::endl;
				}
			}
		}
	}
	else // if (is_connected(fd) == true)
	{
		use_map_function(str, fd);
	}
	return (0);
}
// std::vector<std::string> cmdLine

int Server::check_nick(std::string nickname, int fd, User *user)
{
	std::string to_compare;
	int gotten_fd;
	if (nickname.find(' ') != std::string::npos ||
		nickname.find(',') != std::string::npos ||
		nickname.find('*') != std::string::npos ||
		nickname.find('?') != std::string::npos ||
		nickname.find('@') != std::string::npos ||
		nickname.find('!') != std::string::npos)
	{
		std::cerr << "Invalid character in nickname" << std::endl;
		sendOneRPL(ERR_ERRONEUSNICKNAME(user->getNickname()), fd);
		set_Error_user("ERR_ERRONEUSNICKNAME", fd);
		return 1;
	}

	char c = nickname[0];
	if (c == ':' || c == '#' || c == '&')
	{
		std::cerr << "Invalid character in nickname" << std::endl;
		sendOneRPL(ERR_ERRONEUSNICKNAME(user->getNickname()), fd);
		set_Error_user("ERR_ERRONEUSNICKNAME", fd);
		return (1);
	}
	if (nickname.length() > 200)
	{
		std::cerr << "Nickname is too long" << std::endl;
		sendOneRPL(ERR_ERRONEUSNICKNAME(user->getNickname()), fd);
		set_Error_user("ERR_ERRONEUSNICKNAME", fd);
		return (1);
	}
	for (int i = 0; nickname[i]; i++)
	{
		if (nickname[i] < 32 || nickname[i] > 126)
		{
			std::cerr << "Invalid character in nickname" << std::endl;
			sendOneRPL(ERR_ERRONEUSNICKNAME(user->getNickname()), fd);
			set_Error_user("ERR_ERRONEUSNICKNAME", fd);
			return (1);
		}
	}

	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		User &currentUser = *it;
		to_compare = currentUser.getNickname();
		gotten_fd = currentUser.getFd();
		if (to_compare == nickname && gotten_fd != fd)
		{
			std::cerr << "Same UserName with another user" << to_compare << std::endl;
			sendOneRPL(ERR_NICKNAMEINUSE(user->getNickname()), fd);
			return (1);
		}
		else
		{
			std::string set = user->getNickname();
			user->setNickname(nickname);
			std::cout << set << " changed his nickname to " << user->getNickname() << std::endl;
		}
	}
	return (0);
}
int Server::newUser(int fd, char buffer[1024])
{
	(void)buffer;
	struct epoll_event clientEvent;
	int bytesSent;
	(void)bytesSent;
	User newUser("");
	newUser.setFd(fd);
	_users.push_back(newUser);
	clientEvent.events = EPOLLIN;
	clientEvent.data.fd = _serv.clientSocket;
	if (epoll_ctl(_serv.epollFd, EPOLL_CTL_ADD, _serv.clientSocket, &clientEvent) == -1)
	{
		std::cerr << "Erreur lors de l'ajout du socket client à epoll." << std::endl;
		return 1;
	}
	recieve_data(fd, 1);

	// std::string kenan("kenan");
	// std::string welcomeMessage = RPL_WELCOME(kenan);
	// size_t messageLength = welcomeMessage.length();
	// if (send(fd, welcomeMessage.c_str(), messageLength, 0) == -1)
	// std::cout << "error envoie RPL" << std::endl;
	// bytesSent = send(fd, "Connection to server is success.\n", 34, 0);
	// if (bytesSent < 0)
	// std::cerr << "Erreur lors de l'envoi de la confirmation au client." << std::endl;
	std::cout << "User connected" << std::endl;
	return 0;
}

int Server::checkConnection(int fd, char buffer[1024])
{
	(void)(fd);

	std::vector<std::string> str = get_cmdLine(buffer);
	if (check_password(str) != 0)
	{
		std::cout << "\033[31mMot de passe incorecte\033[0m" << std::endl;
	}
	if (check_nickname(str) != 0)
	{
		std::cout << "\033[31m Nick name incorrecte \033[0m" << std::endl;
	}
	// Afficher le contenu du vecteur
	for (std::vector<std::string>::iterator it = str.begin(); it != str.end(); ++it)
	{
		std::cout << "Élément : " << *it << std::endl;
	}

	// Le reste de votre logique ici...

	return 1;
	// check en premier le mdp(coder la fonction PASS)
	// recup nickname (coder la fonction NICK)
	// recup username firstname lastname (coder la fonction USER)
}

std::vector<std::string> Server::get_cmdLine(char buffer[1024])
{
	std::istringstream iss(buffer);

	std::vector<std::string> words;
	std::string word;

	while (iss >> word)
		words.push_back(word);

	// for (std::vector<std::string>::iterator it = cmdLine.begin(); it != cmdLine.end(); ++it)
	// {
	// 	const std::string& w = *it;
	// 	std::cout << "Mot : " << w << std::endl;
	// }
	return (words);
}

int Server::launchSocket()
{
	int option = 1;
	_serv.serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serv.serverSocket == -1)
	{
		std::cerr << "error socket" << std::endl;
		return 1;
	}
	_serv.serverAddress.sin_family = AF_INET;
	_serv.serverAddress.sin_addr.s_addr = INADDR_ANY;
	_serv.serverAddress.sin_port = htons(_port);
	if (setsockopt(_serv.serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) < 0)
		throw std::runtime_error("Setsockopt failed");
	if (bind(_serv.serverSocket, (struct sockaddr *)&_serv.serverAddress, sizeof(_serv.serverAddress)) == -1)
	{
		std::cerr << "Erreur lors de la liaison de la socket à l'adresse et au port." << std::endl;
		return 1;
	}
	_serv.clientAddrLen = sizeof(_serv.clientAddress);
	if (listen(_serv.serverSocket, 10) == -1) // gerer ici le nombre de connection max a attendre
	{
		std::cerr << "Erreur lors de la mise en écoute de la socket." << std::endl;
		return 1;
	}
	std::cout << "Server socket ready : " << _serv.serverSocket << std::endl;
	return (0);
}
