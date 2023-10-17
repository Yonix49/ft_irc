/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:04:18 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/17 14:49:24 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::~Server()
{
}
// Constructeur
Server::Server()
{
}
Server::Server(int port, const std::string &password)
{
	this->m_port = port;
	this->m_password = password;
}

// Constructeur de copie
Server::Server(const Server &src)
{
	this->m_password = src.m_password;
	this->m_vec = src.m_vec;
	this->m_socket_server = src.m_socket_server;
	this->m_port = src.m_port;
}
// Opérateur d'assignation
Server &Server::operator=(const Server &src)
{
	if (this != &src)
	{
		this->m_password = src.m_password;
		this->m_vec = src.m_vec;
		this->m_socket_server = src.m_socket_server;
		this->m_port = src.m_port;
	}
	return *this;
}

int Server::createServerSocket(int port)
{
	// (void)port;
	// ServerSocket serverSocket;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	socklen_t clientAddrLen;
	int clientSocket;
	int serverSocket;
	char buffer[1024];
	int bytesRead;

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		std::cerr << "error socket" << std::endl;
		return 1;
	}
	std::cout << "server socket connection created" << std::endl;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(port); // Port IRC par défaut
	if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		std::cerr << "Erreur lors de la liaison de la socket à l'adresse et au port." << std::endl;
    	return 1;
	}
	clientAddrLen = sizeof(clientAddress);
	std::cout << "bind success" << std::endl;
	if (listen(serverSocket, 1) == -1) // Limite de 5 connexions en attente
	{
		std::cerr << "Erreur lors de la mise en écoute de la socket." << std::endl;
		return 1;
	}
	std::cout << "listen success" << std::endl;
	clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddrLen);
	if (clientSocket == -1)
	{
    	std::cerr << "Erreur lors de l'acceptation de la connexion entrante." << std::endl;
    	return 1;
	}
	std::cout << "accept succes" << std::endl;
	while (true)
	{
		std::cout << "before recv" << std::endl;
    	bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    	if (bytesRead < 0)
		{
    	    std::cerr << "Erreur lors de la réception de données du client." << std::endl;
    	    break;
    	}
    	if (bytesRead == 0)
		{
    	    std::cout << "Client déconnecté." << std::endl;
    	    break;
    	}
		std::cout << "recv succes" << std::endl;
    	std::string message(buffer, bytesRead);
    	std::cout << "Message du client : " << message << std::endl;
    	// Implémentez ici la logique de l'IRC pour traiter les commandes et gérer la communication avec les clients.
	}
	close (clientSocket);
	return (serverSocket);
}