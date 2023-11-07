/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:35:11 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/07 11:51:08 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../user/user.hpp"

User::User()
{
	_fd = 0;
	_isOperator = 0;
	_isFounder = 0;
	_check_nc = 0;
	_hasQuit = 0;
	_in_server = false;
	_nickname = "";
	_invisible = false;
}

User::User(std::string name) : _nickname(name)
{
	_in_server = false;
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
	_hasQuit = 0;
	_in_server = src._in_server;
	_invisible = src._invisible;
}
// Opérateur d'assignation
User &User::operator=(const User &src)
{
	if (this != &src)
	{
		_nickname = src._nickname;
		_fd = src._fd;
		_isOperator = src._isOperator;
		_check_nc = 0;
		_hasQuit = 0;
		_in_server = src._in_server;
		_invisible = src._invisible;
	}
	return *this;
}
void User::setError(std::string error)
{
	this->_error = error;
}
std::string &User::Get_Error(void)
{
	return (_error);
}
void User::setNickname(std::string name)
{
	_nickname = name;
}

void User::set_in_server(bool _in_server)
{
	this->_in_server = _in_server;
}

void User::setInvisible(bool _invisible)
{
	this->_invisible = _invisible;
}

void	User::setFd(int fd)
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

bool &User::getInvisible()
{
	return (_invisible);
}

void User::incre_nc_check(void)
{
	this->_check_nc++;
}

int &User::get_nc_check(void)
{
	return (_check_nc);
}

int &User::getHasQuit(void)
{
	return (_hasQuit);
}
void	User::setHasQuit(int hasQuit)
{
	_hasQuit = hasQuit;
}

std::string &User::getUsername()
{
	return(_username);
}

std::string &User::getNickname()
{
	return (_nickname);
}

int		&User::getisOperator()
{
	return (_isOperator);
}

int		&User::getisFounder()
{
	return (_isFounder);
}

int		&User::getIsKick()
{
	return (_isKick);
}

void	User::setIsOperator(int isOperator)
{
	_isOperator = isOperator;
	// setChanNick();
}

void	User::setIsFounder(int isFounder)
{
	_isFounder = isFounder;
}

void	User::setIsKick(int isKick)
{
	_isKick = isKick;
}


void	User::addisInvited(std::string channelName)
{
	_isInvited.push_back(channelName);
}

int		User::getisInvited(std::string channelName)
{
	for (std::vector<std::string>::iterator it = _isInvited.begin(); it != _isInvited.end(); ++it)
	{
		if (!(it->compare(channelName)))
			return (1);
	}
	return (0);
}



void	User::sendMessage(char buffer[1024])
{
	int bytesSent;
	bytesSent = send(_fd, buffer, strlen(buffer), 0);
	if (bytesSent < 0)
	{
		std::cerr << "Erreur lors de l'envoi au client " << _fd << std::endl;
	}
}
