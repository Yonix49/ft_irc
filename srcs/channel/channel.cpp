/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:46:17 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/24 17:18:15 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../channel/channel.hpp"


Channel::Channel()
{
}

Channel::Channel(std::string name) : _name(name)
{
}


Channel::~Channel()
{
}

// Constructeur de copie
Channel::Channel(const Channel &src)
{
	_name = src._name;
	_users = src._users;
	_operators = src._operators;
}

// Opérateur d'assignation
Channel &Channel::operator=(const Channel &src)
{
	if (this != &src)
	{
		_name = src._name;
		_users = src._users;
		_operators = src._operators;
	}
	return *this;
}

void	Channel::setName(std::string name)
{
	_name = name;
}


void	Channel::setUsers(std::vector<User> users)
{
	_users = users;
}

void	Channel::setIsOperator(std::vector<User> operators)
{
	_operators = operators;
}

void	Channel::addUser(User user)
{
	_users.push_back(user);
}

void	Channel::sendMessages(char buffer[1024])
{

	for (unsigned long i = 0; i < _users.size(); i++)
		_users[i].sendMessage(buffer);
}
