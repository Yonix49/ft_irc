/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:04:18 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/23 17:27:21 by kgezgin          ###   ########.fr       */
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
	_vec = src._vec;
	_serv = src._serv;
	_port = src._port;
}

// Opérateur d'assignation
Server &Server::operator=(const Server &src)
{
	if (this != &src)
	{
		this->_password = src._password;
		this->_vec = src._vec;
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
	while (true)
	{
		numEvents = epoll_wait(_serv.epollFd, events, 10, -1);
		if (numEvents == -1)
		{
			std::cerr << "Erreur lors de l'appel à epoll_wait." << std::endl;
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
					newUser(_serv.clientSocket);
			}
			else
				recieve_data(fd);
		}
	}
	close (_serv.clientSocket);
	return (_serv.serverSocket);
}


int	Server::recieve_data(int fd)
{
	int		bytesRead;
	int		bytesSent;
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
		std::cout << "Client déconnecté." << std::endl;
		// Code pour retirer le socket client de votre liste de sockets et d'_serv.epollFd si nécessaire.
		close(fd);
	}
	else
	{
		buffer[bytesRead] = '\0';
		std::cout << "Message du client sur le socket " << fd << ": " << buffer << std::endl;
		bytesSent = send(fd, "Message reçu par le serveur.\n", 30, 0);
		if (bytesSent < 0)
			std::cerr << "Erreur lors de l'envoi de la confirmation au client." << std::endl;
		// Implémentez ici la logique de irc
	}
	return (0);
}


int	Server::newUser(int fd)
{
	struct	epoll_event clientEvent;
	int		bytesSent;
	
	clientEvent.events = EPOLLIN;
	clientEvent.data.fd = _serv.clientSocket;
	if (epoll_ctl(_serv.epollFd, EPOLL_CTL_ADD, _serv.clientSocket, &clientEvent) == -1)
	{
		std::cerr << "Erreur lors de l'ajout du socket client à epoll." << std::endl;
		return 1;
	}
	_vec.push_back(clientEvent);
	(void)fd;
	bytesSent = send(fd, "Connection to server is success.\n", 34, 0);
	if (bytesSent < 0)
		std::cerr << "Erreur lors de l'envoi de la confirmation au client." << std::endl;
	std::cout << "User connected" << std::endl;
	// ajouter user a la socket.
	return 0;
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
	if (listen(_serv.serverSocket, 1) == -1) // gerer ici le nombre de connection max a attendre
	{
		std::cerr << "Erreur lors de la mise en écoute de la socket." << std::endl;
		return 1;
	}
	std::cout << "Server socket ready : " << _serv.serverSocket << std::endl;

	return (0);
}





// int Server::createServerSocket(int port)
// {
// 	struct sockaddr_in _serv.serverAddress;
// 	struct sockaddr_in _serv.clientAddress;
// 	socklen_t _serv.clientAddrLen;
// 	int _serv.clientSocket;
// 	int serverSocket;
// 	char buffer[1024];
// 	// memset(buffer, 0, sizeof(buffer));
// 	int bytesRead;
// 	fd_set	set_read;
// 	std::vector<int> _serv.clientSockets; // Liste des sockets clients

// 	FD_ZERO(&set_read);
// 	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
// 	if (serverSocket == -1)
// 	{
// 		std::cerr << "error socket" << std::endl;
// 		return 1;
// 	}
// 	std::cout << "server socket connection created" << std::endl;
// 	serverAddress.sin_family = AF_INET;
// 	serverAddress.sin_addr.s_addr = INADDR_ANY;
// 	serverAddress.sin_port = htons(port); // Port IRC par défaut
// 	int option = 1;
// 	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) < 0)
// 		throw std::runtime_error("Setsockopt failed");
// 	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
// 	{
// 		std::cerr << "Erreur lors de la liaison de la socket à l'adresse et au port." << std::endl;
//     	return 1;
// 	}
// 	clientAddrLen = sizeof(clientAddress);
// 	std::cout << "bind success" << std::endl;
	
// 	if (listen(serverSocket, 1) == -1) // Limite de 5 connexions en attente
// 	{
// 		std::cerr << "Erreur lors de la mise en écoute de la socket." << std::endl;
// 		return 1;
// 	}
// 	std::cout << "server is listening on port "<< port << std::endl;
	
// 	FD_SET(serverSocket, &set_read);
// 	int maxFd = serverSocket;
// 	std::cout << "Server socket ready : " << serverSocket << std::endl;
// 	while (true)
// 	{
// 		std::cout << "here" << std::endl;
// 		int selectResult = select(maxFd + 1, &set_read, NULL, NULL, NULL);
// 		if (selectResult == -1) {
// 			std::cerr << "Error using select." << std::endl;
// 			return 1;
// 		}
// 		std::cout << "here2" << std::endl;
// 		for (int i = 0 ; i <= maxFd ; i++)
// 		{
// 			if (FD_ISSET(i, &set_read))
// 			{
// 				_serv.clientSocket = accept(i, (struct sockaddr*)&clientAddress, &clientAddrLen);
// 				if (clientSocket == serverSocket)
// 				{
// 					if (clientSocket == -1)
// 					{
// 						std::cerr << "Erreur lors de l'acceptation de la connexion entrante." << std::endl;
// 						return 1;
// 					}
// 					int writed = write(clientSocket, "salut", 5);
// 					std::cout << "writed = " << writed << std::endl;
					
// 					clientSockets.push_back(clientSocket);
// 					FD_SET(clientSocket, &set_read);
// 					if (clientSocket > maxFd) {
// 							maxFd = clientSocket;
// 						}
// 					// for (std::vector<int>::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it)
// 					// {
// 					// 	int clientSock = *it;
// 					// 	FD_SET(clientSock, &set_read);
// 					// 	if (clientSock > maxFd) {
// 					// 		maxFd = clientSock;
// 					// 	}
// 					// }
// 					std::cout << "accept success new socket client " << clientSocket << std::endl;
// 				}
// 				else
// 				{
// 					std::cout << "before recv" << std::endl;
// 					bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
// 					buffer[bytesRead] = '\0';
// 					if (bytesRead < 0)
// 					{
// 						std::cerr << "Erreur lors de la réception de données du client." << std::endl;
// 						break;
// 					}
// 					if (bytesRead == 0)
// 					{
// 						std::cout << "Client déconnecté." << std::endl;
// 						break;
// 					}
// 					std::cout << "recv succes" << std::endl;
// 					std::cout << "buffer = " << buffer << std::endl;
// 					// std::string message(buffer, bytesRead);
// 					// std::cout << "Message du client : " << message << std::endl;
// 				}
// 			}
// 		}
// 		// Implémentez ici la logique de l'IRC pour traiter les commandes et gérer la communication avec les clients.
// 	}
// 	close (clientSocket);
// 	// close (serverSocket);
// 	return (serverSocket);
// }



// int Server::createServerSocket(int port)
// {
// 	// (void)port;
// 	// ServerSocket serverSocket;
// 	struct sockaddr_in serverAddress;
// 	struct sockaddr_in clientAddress;
// 	socklen_t clientAddrLen;
// 	int clientSocket;
// 	int serverSocket;
// 	char buffer[1024];
// 	// memset(buffer, 0, sizeof(buffer));
// 	int bytesRead;
// 	// fd_set	set_read;
// 	fd_set readFds;

// 	// FD_ZERO(&set_read);
// 	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
// 	if (serverSocket == -1)
// 	{
// 		std::cerr << "error socket" << std::endl;
// 		return 1;
// 	}
// 	std::cout << "server socket connection created" << std::endl;
// 	// FD_SET(serverSocket, &set_read);
// 	serverAddress.sin_family = AF_INET;
// 	serverAddress.sin_addr.s_addr = INADDR_ANY;
// 	serverAddress.sin_port = htons(port); // Port IRC par défaut
// 	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
// 	{
// 		std::cerr << "Erreur lors de la liaison de la socket à l'adresse et au port." << std::endl;
//     	return 1;
// 	}
// 	clientAddrLen = sizeof(clientAddress);
	

// 	std::cout << "bind success" << std::endl;
// 	if (listen(serverSocket, 1) == -1) // Limite de 5 connexions en attente
// 	{
// 		std::cerr << "Erreur lors de la mise en écoute de la socket." << std::endl;
// 		return 1;
// 	}
// 	std::cout << "server is listening on port "<< port << std::endl;
// 	// clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddrLen);
// 	// if (clientSocket == -1)
// 	// {
//     // 	std::cerr << "Erreur lors de l'acceptation de la connexion entrante." << std::endl;
//     // 	return 1;
// 	// }
// 	// FD_SET(clientSocket, &set_read);
// 	// while (true)
// 	// {
// 	// std::vector<int> clientSockets; // Liste des sockets clients

// 	FD_ZERO(&readFds);
// 	while (true)
// 	{
// 		// fd_set readFds;
// 		FD_SET(serverSocket, &readFds);
// 		// int maxFd = serverSocket;
		
// 		// for (std::vector<int>::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it) {
// 		// 	int clientSock = *it;
// 		// 	FD_SET(clientSock, &readFds);
// 		// 	if (clientSock > maxFd) {
// 		// 		maxFd = clientSock;
// 		// 	}
// 		// }
// 		struct timeval timeout;
// 		timeout.tv_sec = 50; // Timeout de 5 secondes (à ajuster selon vos besoins)
// 		timeout.tv_usec = 0;

// 		int selectResult = select(2 + 1, &readFds, NULL, NULL, &timeout);
// 		if (selectResult == -1) {
// 			std::cerr << "Error using select." << std::endl;
// 			return 1;
// 		}
// 		// int rc = select(1, &set_read, NULL, NULL, 0);
// 		// if (rc < 0)
// 		// {
// 		// 	std::cerr << "error on select " << std::endl;
// 		// 	return (1);
// 		// }
// 		// if (FD_ISSET(serverSocket, &readFds))
// 		// {
// 			clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddrLen);
// 			if (clientSocket == -1)
// 			{
// 				std::cerr << "Erreur lors de l'acceptation de la connexion entrante." << std::endl;
// 				return 1;
// 			}
// 		// }
// 		// FD_SET(clientSocket, &readFds);
// 		std::cout << "accept succes" << std::endl;
// 		std::cout << "before recv" << std::endl;
// 		bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
// 		if (bytesRead < 0)
// 		{
// 			std::cerr << "Erreur lors de la réception de données du client." << std::endl;
// 			break;
// 		}
// 		if (bytesRead == 0)
// 		{
// 			std::cout << "Client déconnecté." << std::endl;
// 			break;
// 		}
// 		std::cout << "recv succes" << std::endl;
// 		std::string message(buffer, bytesRead);
// 		std::cout << "Message du client : " << message << std::endl;
// 		// Implémentez ici la logique de l'IRC pour traiter les commandes et gérer la communication avec les clients.
// 	}
// 	close (clientSocket);
// 	return (serverSocket);
// }


// int Server::createServerSocket(int port)
// {
// 	struct sockaddr_in serverAddress;
// 	struct sockaddr_in clientAddress;
// 	socklen_t clientAddrLen;
// 	int clientSocket;
// 	int serverSocket;
// 	char buffer[1024];

// 	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
// 	if (serverSocket == -1) {
// 		std::cerr << "Error creating socket" << std::endl;
// 		return 1;
// 	}

// 	serverAddress.sin_family = AF_INET;
// 	serverAddress.sin_addr.s_addr = INADDR_ANY;
// 	serverAddress.sin_port = htons(port);

// 	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
// 		std::cerr << "Error binding socket to address and port." << std::endl;
// 		return 1;
// 	}

// 	clientAddrLen = sizeof(clientAddress);

// 	if (listen(serverSocket, 1) == -1) {
// 		std::cerr << "Error listening on the socket." << std::endl;
// 		return 1;
// 	}

// 	std::cout << "Server listening on port " << port << std::endl;

// 	std::vector<int> clientSockets; // Liste des sockets clients

// 	while (true) {
// 		fd_set readFds;
// 		FD_ZERO(&readFds);
// 		FD_SET(serverSocket, &readFds);
// 		int maxFd = serverSocket;

// 		for (std::vector<int>::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it) {
// 			int clientSock = *it;
// 			FD_SET(clientSock, &readFds);
// 			if (clientSock > maxFd) {
// 				maxFd = clientSock;
// 			}
// 		}

// 		struct timeval timeout;
// 		timeout.tv_sec = 5; // Timeout de 5 secondes (à ajuster selon vos besoins)
// 		timeout.tv_usec = 0;

// 		int selectResult = select(maxFd + 1, &readFds, NULL, NULL, &timeout);

// 		if (selectResult == -1) {
// 			std::cerr << "Error using select." << std::endl;
// 			return 1;
// 		}

// 		if (FD_ISSET(serverSocket, &readFds)) {
// 			clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddrLen);
// 			if (clientSocket == -1) {
// 				std::cerr << "Error accepting incoming connection." << std::endl;
// 				return 1;
// 			}

// 			std::cout << "Accepted connection from client." << std::endl;
// 			clientSockets.push_back(clientSocket);
// 		}

// 		for (std::vector<int>::iterator it = clientSockets.begin(); it != clientSockets.end();) {
// 			int clientSock = *it;
// 			if (FD_ISSET(clientSock, &readFds)) {
// 				int bytesRead = recv(clientSock, buffer, sizeof(buffer), 0);
// 				if (bytesRead < 0) {
// 					std::cerr << "Error receiving data from client." << std::endl;
// 					// Handle the error or disconnect the client.
// 					close(clientSock);
// 					it = clientSockets.erase(it);
// 				} else if (bytesRead == 0) {
// 					std::cout << "Client disconnected." << std::endl;
// 					close(clientSock);
// 					it = clientSockets.erase(it);
// 				} else {
// 					std::string message(buffer, bytesRead);
// 					std::cout << "Message from client: " << message << std::endl;
// 					// Implement IRC logic to handle commands and client communication here.
// 					++it;
// 				}
// 			} else {
// 				++it;
// 			}
// 		}
// 	}

// 	close(serverSocket);
// 	return 0;
// }

// int main() {
//     Server server;
//     int port = 6667;
//     server.createServerSocket(port);
//     return 0;
// }
