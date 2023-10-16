/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:04:18 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/16 19:36:05 by mhajji-b         ###   ########.fr       */
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

ServerSocket Server::createServerSocket(int port)
{
	(void)(port);
	ServerSocket serverSocketObj;

	return (serverSocketObj);
}