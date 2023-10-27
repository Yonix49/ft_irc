/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:35:11 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/27 20:12:37 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../user/user.hpp"

User::User()
{
	_check_nc = 0;
	_in_server = false;

}

User::User(std::string name) : _nickname(name)
{
}

User::~User()
{
}

// Constructeur de copie
User::User(const User &src)
{
	_nickname = src._nickname;
	_fd = src._fd;
	_isOperator = src._isOperator;
	_check_nc = 0;
}

// Opérateur d'assignation
User &User::operator=(const User &src)
{
	if (this != &src)
	{
		_nickname = src._nickname;
		_fd = src._fd;
		_isOperator = src._isOperator;
	}
	return *this;
}

void User::setNickname(std::string name)
{
	_nickname = name;
}
void User::set_in_server(bool _in_server)
{
	this->_in_server = _in_server;
}

void User::setFd(int fd)
{
	_fd = fd;
}
void User::setRealname(std::string name)
{
	_realname = name;
}

void User::setUsername(std::string name)
{
	_username = name;
}
int &User::getFd()
{
	return (_fd);
}
bool &User::get_check_in_server()
{
	return (_in_server);
}
void User::incre_nc_check(void)
{
	this->_check_nc++;
}
int &User::get_nc_check(void)
{
	return (_check_nc);
}

std::string &User::getNickname()
{
	return (_nickname);
}

void User::setIsOperator(int isOperator)
{
	_isOperator = isOperator;
}

void User::sendMessage(char buffer[1024])
{
	int bytesSent;
	bytesSent = send(_fd, buffer, strlen(buffer), 0);
	if (bytesSent < 0)
	{
		std::cerr << "Erreur lors de l'envoi au client " << _fd << std::endl;
	}
}
