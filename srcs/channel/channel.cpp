/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:46:17 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/30 16:39:10 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../channel/channel.hpp"


Channel::Channel()
{
	_limitUsers = 0;
	_nbUsers = 0;
	_isInviteOnlyMod = false;
}

Channel::Channel(std::string name) : _name(name)
{
	_limitUsers = 0;
	_nbUsers = 0;
	_isInviteOnlyMod = false;
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
	_limitUsers = src._limitUsers;
	_isInviteOnlyMod = src._isInviteOnlyMod;
	_topic = src._topic;
	_invited = src._invited;
	_nbUsers = src._nbUsers;
	_password = src._password;
}

// Opérateur d'assignation
Channel &Channel::operator=(const Channel &src)
{
	if (this != &src)
	{
		_name = src._name;
		_users = src._users;
		_operators = src._operators;
		_limitUsers = src._limitUsers;
		_isInviteOnlyMod = src._isInviteOnlyMod;
		_topic = src._topic;
		_invited = src._invited;
		_nbUsers = src._nbUsers;
		_password = src._password;
	}
	return *this;
}

void	Channel::setName(std::string name)
{
	_name = name;
}

void	Channel::setTopic(std::string topic)
{
	_topic = topic;
}

void	Channel::setLimitUsers(int limit)
{
	_limitUsers = limit;
}

std::string	&Channel::getName(void)
{
	return (_name);
}

std::string	Channel::getTopic(void)
{
	return (_topic);
}

std::string	Channel::getPassword(void)
{
	return (_password);
}


int	Channel::getLimitUsers(void)
{
	return (_limitUsers);
}

bool	Channel::getIsInviteOnlyMod(void)
{
	return (_isInviteOnlyMod);
}

int	Channel::getNbUsers(void)
{
	return (_nbUsers);
}


std::string		Channel::getListUsers(void)
{
	std::string userInfo;
	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->getisOperator() > 0)
			userInfo += '@';
		else
			userInfo += '+';
		userInfo += it->getNickname();
		userInfo += ' ';
	}
	return (userInfo);
}


void	Channel::setUsers(std::vector<User> users)
{
	_users = users;
}

void	Channel::setIsOperator(std::vector<User> operators)
{
	_operators = operators;
}

void	Channel::setPassword(std::string password)
{
	_password = password;
}

void	Channel::setNbUsers(int nbUsers)
{
	_nbUsers = nbUsers;
}

void	Channel::setIsInviteOnlyMod(bool nb)
{
	_isInviteOnlyMod = nb;
}

void	Channel::addInvitedUser(std::string nickname)
{
	_invited.push_back(nickname);
}


void	Channel::updateInvited(std::string nickname)
{
	for (std::vector<std::string>::iterator it = _invited.begin(); it != _invited.end(); ++it)
	{
		if (!nickname.compare(*it))
		{
			_invited.erase(it);
			return ;
		}
	}
}


int	Channel::addUser(User user, int isOperator, std::string channelName, int fd)
{
	(void)fd;
	std::string newnick;
	std::cout << "ADDUSER IS CALLED" << std::endl;
	for (std::vector<User>::iterator it = _users.begin(); it < _users.end(); ++it)
	{
		if (it->getNickname() == user.getNickname())
		{
			// rpl user already in channel
			return (-1);
		}
	}
	
	if (isOperator > 0)
	{
		newnick = "@" + user.getNickname();
		_operators.push_back(user);
		if (isOperator == 2)
			user.setIsFounder(1);
	}
	else
	{
		newnick = "+" + user.getNickname();
		// si le user est invite, on le sors de la liste des invites
		updateInvited(user.getNickname()); 
	}

	_users.push_back(user);
	_nbUsers = _nbUsers + 1;
	sendRPLtoChan(RPL_NAMREPLY(newnick, channelName, getListUsers()));
	// if (getTopic().empty())
	// 	sendOneRPL(RPL_TOPIC(user.getNickname(), channelName, getTopic()), fd);
	sendRPLtoChan(RPL_ENDOFNAMES(user.getNickname(), channelName));
	return (0);
}

int	Channel::isUserinchan(std::string nickname, int type)
{
	if (type == 0)
		return (findUser(nickname, _users));
	else if (type == 1)
		return (findUser(nickname, _operators));
	return (-1);
}


void	Channel::sendRPLtoChan(std::string rpl)
{

	for (unsigned long i = 0; i < _users.size(); i++)
		sendOneRPL(rpl, _users[i].getFd());
}
