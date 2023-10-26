/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:04:18 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/26 22:28:15 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::~Server()
{
	_serv.clientSocket = 0;
	_serv.serverSocket = 0;
}
// Constructeur
Server::Server()
{
	_checking_nc = 0;
}
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

void Server::setPort(long int port)
{
	_port = port;
}
void Server::setPassword(std::string password)
{
	_password = password;
}

User *Server::getUserNo(int fd)
{
	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->getFd() == fd)
			return &(*it);
	}
	return (NULL);
}
std::string Server::get_password(void)
{
	return (_password);
}

int Server::createServerSocket()
{
	int numEvents;
	struct epoll_event event;
	struct epoll_event events[10]; // Nombre maximal d'événements à gérer
	int fd;
	int isNewUser;

	_serv.epollFd = epoll_create(1);
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
	else
	{
		buffer[bytesRead] = '\0';
		// std::cout << "Message du client sur le socket " << fd << ": " << buffer << std::endl;
		// if (isNewUser)
		// {
		nc_check(buffer, fd);
		// if (!checkConnection(fd, buffer)) // check le mdp, recuperer le nickname et toutes les infos necessaires
		// {
		// 	std::cout << "ici check connection" << std::endl;
		// 	return (-1);
		// 	//     // temporairement on envoie le message suivant mais sinon il faut envoyer le bon rpl dans
		// 	//     // check_connection
		// 	//     bytesSent = send(fd, "Connection to server is rejected.\n", 34, 0);
		// 	//     return (-1);
		// }
		// 	std::cout << "nickname = " << _users.back().getNickname() << std::endl;
		// 	std::string welcomeMessage = RPL_WELCOME(_users.back().getNickname());
		// 	size_t messageLength = welcomeMessage.length();
		// 	if (send(fd, welcomeMessage.c_str(), messageLength, 0) == -1)
		// 		std::cout << "error envoie RPL" << std::endl;
		// }
		// else
		// 	launchCmd(buffer, fd);
	}
	return (0);
}
int Server::nc_check(char buffer[1024], int fd)
{
	std::string str(buffer);	 // Convertir le buffer en std::string
	std::istringstream iss(str); // Utiliser un std::istringstream pour diviser en mots
	std::vector<std::string> words;

	std::string word;
	while (iss >> word)
	{
		words.push_back(word);
	}

	int gotten_fd = 0;

	User *user = NULL; // Déclarer un pointeur vers un utilisateur

	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		User &currentUser = *it;
		gotten_fd = currentUser.getFd();
		if (gotten_fd == fd)
		{
			user = &currentUser; // Stocker la référence à l'utilisateur correspondant
		}
	}
	if (user && user->get_nc_check() <= 2)
	{
		if (!words.empty() && words.size() > 0)
		{
			std::cout << "password " << _password << std::endl;
			if (words[0] == "PASS" && user->get_nc_check() == 0)
			{
				if (words.size() == 2 && words[1] == _password.c_str())
				{
					std::cout << "good password" << std::endl;
					user->incre_nc_check();
				}
				else
				{
					// ERR_PASSWDMISMATCH
				}
			}
			else if (words[0] == "NICK" && user->get_nc_check() == 1)
			{
				std::cout << "je suis entrer dans NICK" << std::endl;
				if (words.size() == 2)
				{
					if (check_nick(words[1], fd, user) != 0)
					{
						std::cerr << "Mauvais nickname" << std::endl;
					}
					else
					{
						user->incre_nc_check();
						user->setNickname(words[1]);
						std::cout << "good NICK " << words[1] << std::endl;
					}
					// }
					// develop la logique ici
				}
			}
			else if (words[0] == "USER")
			{ // develop la logique ici

				std::cout << "je suis entrer dans USER" << std::endl;
				if (check_user(fd, user, words) != 0)
				{
					std::cerr << "Erreur commande USER" << std::endl;
				}
				else
				{
					user->incre_nc_check();
				}
				// Je dois check le bon USER ici
			}
		}
	}
	return 0;
}
int Server::check_user(int fd, User *user, std::vector<std::string> words)
{

	// for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it)
	// {
	// 	std::cout << *it << std::endl;
	// }
	if (words.size() == 5)
	{
		std::cout << "je suis la" << std::endl;
		// std::cout << "le username rentrer avant op == " << words[1] << std::endl;
		if (words[1].empty() || words[1].length() <= 1)
		{
			std::cout << "je suis la 2" << words[1] << std::endl;
			return (1);
			// Le serveur rejette avec rep ERR_NEEDMOREPARAMS
		}
		if (words[1].length() > 10) // Username // USERLEN=10
		{
			words[1] = words[1].substr(0, 10);
			user->setUsername(words[1]);
			std::cout << words[1] << " length == " << words[1].length() << std::endl;
		}
		else if (words[2] != "0" && words[3] != "*")
		{
			std::cout << "je suis la 3" << std::endl;

			std::cout << "mauvais parametres" << std::endl;
			return (1);
		}
		else if (!words[4].empty())
		{
			if (words[4].find(' ') != std::string::npos)
			{
				words[4] = ":" + words[4];
			}
			user->setRealname(words[4]);
			std::cout << "real name " << words[4] << std::endl;
		}
	}
	else
	{
		std::cout << "je suis la 4" << std::endl;
		std::cout << "Usage of USER Parameters: <username> 0 * <realname>" << std::endl;
		// ERR_NEEDMOREPARAMS
		return (1);
	}
	// si le gars a deja utiliser USER au debut lors de nc msg derror si il ressaie de l'utiliser ERR_ALREADYREGISTERED
	(void)(user);
	(void)(fd);
	return (0);
}

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
		return 1;
	}

	char c = nickname[0];
	if (c == ':' || c == '#' || c == '&')
	{
		std::cerr << "Invalid character in nickname" << std::endl;
		return (1);
	}
	if (nickname.length() > 200)
	{
		std::cerr << "Nickname is too long" << std::endl;
		return (1);
	}
	for (int i = 0; nickname[i]; i++)
	{
		if (nickname[i] < 32 || nickname[i] > 126)
		{
			std::cerr << "Invalid character in nickname" << std::endl;
			return 1;
		}
	}

	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		User &currentUser = *it;
		to_compare = currentUser.getNickname();
		gotten_fd = currentUser.getFd();
		std::cout << "compare" << to_compare << std::endl;
		if (to_compare == nickname && gotten_fd != fd)
		{
			std::cerr << "Same UserName with another user" << to_compare << std::endl;
			return (1);
		}
		else
		{
			std::cout << "nickname get replace by "
					  << "nickname" << std::endl;
			user->setNickname(nickname);
			std::cout << user->getNickname() << std::endl;
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
	std::stringstream ss;
	ss << "User" << fd;
	std::string nickname(ss.str());
	User newUser(nickname);
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

int Server::launchCmd(char buffer[1024], int fd)
{
	std::vector<std::string> cmdLine;

	cmdLine = get_cmdLine(buffer);
	if (!cmdLine[0].compare("/JOIN") || !cmdLine[0].compare("/join"))
		join(cmdLine, fd);
	return (0);
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
