/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:35:11 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/25 16:45:49 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../user/user.hpp"


User::User()
{
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



void	User::setIsOperator(int isOperator)
{
	_isOperator = isOperator;
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
