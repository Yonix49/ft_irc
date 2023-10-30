/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:04:18 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/30 17:26:23 by kgezgin          ###   ########.fr       */
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
}
Server::Server(int port, const std::string &password)
{
	_port = port;
	_password = password;
	_serv.clientSocket = 0;
	_serv.serverSocket = 0;
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

void	Server::setPort(long int port)
{
	_port = port;
}

User	*Server::getUserNo(int fd)
{
	for (std::vector<User>::iterator it = _users.begin() ; it != _users.end() ; ++it)
	{
		if (it->getFd() == fd)
			return &(*it);
	}
	return (NULL);
}

User	*Server::getUserString(std::string nickname)
{
	for (std::vector<User>::iterator it = _users.begin() ; it != _users.end() ; ++it)
	{
		if (!it->getNickname().compare(nickname))
			return &(*it);
	}
	return (NULL);
}


int	findUser(std::string str, std::vector<User> users)
{
	int	i = 0;
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (!str.compare((*it).getNickname()))
			return (i);
		i++;
	}
	return (-1);
}


int Server::createServerSocket()
{
	int 				numEvents;
	struct epoll_event	event;
	struct epoll_event	events[10];  // Nombre maximal d'événements à gérer
	int					fd;
	int					isNewUser;

	_serv.epollFd = epoll_create(1);
	if (_serv.epollFd == -1)
	{
		std::cerr << "Erreur lors de la création d'epoll." << std::endl;
		return 1;
	}
	launchSocket();

	event.events = EPOLLIN;
	event.data.fd = _serv.serverSocket;  // Ajoutez le socket du serveur à l'instance epoll
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
				_serv.clientSocket = accept(_serv.serverSocket, (struct sockaddr*)&_serv.clientAddress, &_serv.clientAddrLen);
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
				struct	epoll_event zero;
				zero.events = 0;
				zero.data.fd = -1;
				recieve_data(fd, 0, zero);
				isNewUser = 0;
			}
		}
	}
	close (_serv.clientSocket);
	return (_serv.serverSocket);
}


int	Server::recieve_data(int fd, int isNewUser, struct	epoll_event clientEvent)
{
	(void)isNewUser;
	int		bytesRead;
	// int		bytesSent;
	int		bytesSent2;
	char	buffer[1024];

	bytesRead = recv(fd, buffer, sizeof(buffer), 0);
	if (bytesRead < 0)
		std::cerr << "Erreur lors de la réception de données du client." << std::endl;
	else if (bytesRead == 0)
	{
		bytesSent2 = send(fd, "Deconnection reçu par le serveur.\n", 34, 0);
		if (bytesSent2 < 0)
			std::cerr << "Erreur lors de l'envoi de la confirmation au client." << std::endl;
		if (epoll_ctl(_serv.epollFd, EPOLL_CTL_DEL, fd, &clientEvent) == -1)
		{
			std::cerr << "Erreur lors de la suppression du socket client à epoll." << std::endl;
			return 1;
		}
		std::cout << "User disconnected" << std::endl;
		// Code pour retirer le socket client de votre liste de sockets et d'_serv.epollFd si nécessaire.
		close(fd);
	}
	else
	{
		buffer[bytesRead] = '\0';
		std::cout << fd << ": " << buffer << std::endl;
		if (isNewUser)
		{
			if (!checkConnection(fd, buffer)) // check le mdp, recuperer le nickname et toutes les infos necessaires
			{
				std::cout << "ici check connection" << std::endl;
				return (-1);
				//     // temporairement on envoie le message suivant mais sinon il faut envoyer le bon rpl dans
				//     // check_connection
				//     bytesSent = send(fd, "Connection to server is rejected.\n", 34, 0);
				//     return (-1);
			}
			std::cout << "nickname = " << _users.back().getNickname() << std::endl;
			std::string welcomeMessage = RPL_WELCOME(_users.back().getNickname());
			size_t messageLength = welcomeMessage.length();
			if (send(fd, welcomeMessage.c_str(), messageLength, 0) == -1)
				std::cout << "error envoie RPL" << std::endl;
		}
		else
			launchCmd(buffer, fd);
	}
	return (0);
}

int	Server::newUser(int fd, char buffer[1024])
{
	(void)buffer;
	struct	epoll_event clientEvent;
	int					bytesSent;
	(void)bytesSent;
	std::stringstream 	ss;
    ss << "User" << fd;
    std::string			nickname(ss.str());
	User				newUser(nickname);
	newUser.setFd(fd);
	_users.push_back(newUser);
	clientEvent.events = EPOLLIN;
	clientEvent.data.fd = _serv.clientSocket;
	if (epoll_ctl(_serv.epollFd, EPOLL_CTL_ADD, _serv.clientSocket, &clientEvent) == -1)
	{
		std::cerr << "Erreur lors de l'ajout du socket client à epoll." << std::endl;
		return 1;
	}
	recieve_data(fd, 1, clientEvent);
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


int	Server::launchCmd(char buffer[1024], int fd)
{
	std::vector<std::string> cmdLine;

	cmdLine = get_cmdLine(buffer);
	if (cmdLine.empty() == true)
	{
		std::cout << "EMPTY LINE DETECTED" << std::endl;	
		return (-1);	// command line empty
	}
	if (!cmdLine[0].compare("JOIN") || !cmdLine[0].compare("join"))
		join(cmdLine, fd);
	if (!cmdLine[0].compare("MODE") || !cmdLine[0].compare("mode"))
		mode(cmdLine, fd);
	if (!cmdLine[0].compare("INVITE") || !cmdLine[0].compare("invite"))
		invite(cmdLine, fd);
	// KICK
	// PART
	// QUIT
	// PRIVMSG
	// CHANMSG
	// NOTICE
	return (0);
}


std::vector<std::string>	Server::get_cmdLine(char buffer[1024])
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


int	Server::launchSocket()
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
	if (bind(_serv.serverSocket, (struct sockaddr*)&_serv.serverAddress, sizeof(_serv.serverAddress)) == -1)
	{
		std::cerr << "Erreur lors de la liaison de la socket à l'adresse et au port." << std::endl;
		return 1;
	}
	_serv.clientAddrLen = sizeof(_serv.clientAddress);
	if (listen(_serv.serverSocket, 50) == -1) // gerer ici le nombre de connection max a attendre
	{
		std::cerr << "Erreur lors de la mise en écoute de la socket." << std::endl;
		return 1;
	}
	std::cout << "Server socket ready : " << _serv.serverSocket << std::endl;
	return (0);
}
