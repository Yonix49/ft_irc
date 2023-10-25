/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:04:18 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/25 13:38:06 by kgezgin          ###   ########.fr       */
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
					isNewUser = 1;
			}
			else
			{
					// newUser(_serv.clientSocket);
				recieve_data(fd, isNewUser);
				isNewUser = 0;
			}
		}
	}
	close (_serv.clientSocket);
	return (_serv.serverSocket);
}


int	Server::recieve_data(int fd, int isNewUser)
{
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
		std::cout << "User disconnected" << std::endl;
		// Code pour retirer le socket client de votre liste de sockets et d'_serv.epollFd si nécessaire.
		close(fd);
	}
	else
	{
		buffer[bytesRead] = '\0';
		std::cout << "Message du client sur le socket " << fd << ": " << buffer << std::endl;
		if (isNewUser)
			newUser(fd, buffer);
		// Implémentez ici la logique de irc
	}
	return (0);
}


int	Server::newUser(int fd, char buffer[1024])
{
	struct	epoll_event clientEvent;
	int					bytesSent;
	User				newUser;
	
	if (!checkConnection(fd, buffer)) // check le mdp, recuperer le nickname et toutes les infos necessaires
	{
		// temporairement on envoie le message suivant mais sinon il faut envoyer le bon rpl dans
		// check_connection
		bytesSent = send(fd, "Connection to server is rejected.\n", 34, 0);
		return (-1);
	}
	_users.push_back(newUser);
	clientEvent.events = EPOLLIN;
	clientEvent.data.fd = _serv.clientSocket;
	if (epoll_ctl(_serv.epollFd, EPOLL_CTL_ADD, _serv.clientSocket, &clientEvent) == -1)
	{
		std::cerr << "Erreur lors de l'ajout du socket client à epoll." << std::endl;
		return 1;
	}
	
	bytesSent = send(fd, "Connection to server is success.\n", 34, 0);
	if (bytesSent < 0)
		std::cerr << "Erreur lors de l'envoi de la confirmation au client." << std::endl;
	std::cout << "User connected" << std::endl;
	return 0;
}


int	Server::checkConnection(int fd, char buffer[1024])
{
	// check en premier le mdp(coder la fonction PASS)
	// recup nickname (coder la fonction NICK)
	// recup username firstname lastname (coder la fonction USER)
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
	if (listen(_serv.serverSocket, 10) == -1) // gerer ici le nombre de connection max a attendre
	{
		std::cerr << "Erreur lors de la mise en écoute de la socket." << std::endl;
		return 1;
	}
	std::cout << "Server socket ready : " << _serv.serverSocket << std::endl;

	Channel		newChannel("channel_1");
	
	_channels.push_back(newChannel);
	return (0);
}

