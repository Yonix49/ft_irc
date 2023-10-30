/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:35:11 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/30 16:21:57 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../user/user.hpp"


User::User()
{
	_fd = 0;
	_isOperator = 0;
	_isFounder = 0;
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

void	User::setNickname(std::string name)
{
	_nickname = name;
}

// void	User::setChanNick()
// {
// 	if (_isOperator == 0)
// 		_chanNick = _nickname;
// 	else
// 		_chanNick = '@' + _nickname;
// }

void	User::setFd(int fd)
{
	_fd = fd;
}

int	&User::getFd()
{
	return (_fd);
}

std::string		&User::getNickname()
{
	return (_nickname);
}

int		User::getisOperator()
{
	return (_isOperator);
}

int		User::getisFounder()
{
	return (_isFounder);
}

void	User::setIsOperator(int isOperator)
{
	_isOperator = isOperator;
	// setChanNick();
}

void	User::setIsFounder(int isFounder)
{
	_isFounder = isFounder;
	// setChanNick();
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
