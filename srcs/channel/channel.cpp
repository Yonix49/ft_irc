/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:46:17 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/01 17:24:12 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../channel/channel.hpp"


Channel::Channel()
{
	_limitUsers = 0;
	_nbUsers = 0;
	_mode_i = false;
	_mode_t = false;
	_mode_k = false;
	_mode_o = false;
	_mode_l = false;

}

Channel::Channel(std::string name) : _name(name)
{
	_limitUsers = 0;
	_nbUsers = 0;
	_mode_i = false;
	_mode_t = false;
	_mode_k = false;
	_mode_o = false;
	_mode_l = false;

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
	_topic = src._topic;
	_invited = src._invited;
	_nbUsers = src._nbUsers;
	_password = src._password;
	_mode_i = src._mode_i;
	_mode_t = src._mode_t;
	_mode_k = src._mode_k;
	_mode_o = src._mode_o;
	_mode_l = src._mode_l;
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
		_topic = src._topic;
		_invited = src._invited;
		_nbUsers = src._nbUsers;
		_password = src._password;
		_mode_i = src._mode_i;
		_mode_t = src._mode_t;
		_mode_k = src._mode_k;
		_mode_o = src._mode_o;
		_mode_l = src._mode_l;
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

std::string	&Channel::getTopic(void)
{
	return (_topic);
}

std::string	&Channel::getPassword(void)
{
	return (_password);
}


int	Channel::getLimitUsers(void)
{
	return (_limitUsers);
}

bool	Channel::getMode_i(void)
{
	return (_mode_i);
}

bool	Channel::getMode_t(void)
{
	return (_mode_t);
}

bool	Channel::getMode_k(void)
{
	return (_mode_k);
}

bool	Channel::getMode_o(void)
{
	return (_mode_o);
}

bool	Channel::getMode_l(void)
{
	return (_mode_l);
}

int	Channel::getNbUsers(void)
{
	return (_nbUsers);
}

std::string		Channel::getModes()
{
	std::string res;

	res = "mode_i = " + getModes_2(getMode_i()) + "\n";
	res += "mode_t = " + getModes_2(getMode_t()) + "\n";
	res += "mode_k = " + getModes_2(getMode_k()) + "\n";
	res += "mode_l = " + getModes_2(getMode_l()) + "\n";
	return (res);
}

std::string		Channel::getModes_2(bool i)
{
	if (i == true)
		return ("true");
	return ("false");
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



void	Channel::setMode_i(bool nb)
{
	_mode_i = nb;
}

void	Channel::setMode_t(bool nb)
{
	_mode_t = nb;
}

void	Channel::setMode_k(bool nb)
{
	_mode_k = nb;
}

void	Channel::setMode_o(bool nb)
{
	_mode_o = nb;
}

void	Channel::setMode_l(bool nb)
{
	_mode_l = nb;
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
	// std::cout << "ADDUSER IS CALLED" << std::endl;
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
	sendOneRPL(RPL_JOIN(user.getNickname(), channelName), fd);
	if (getTopic().empty() == true)
		sendOneRPL(RPL_NOTOPIC(user.getNickname(), channelName), fd);
	else
		sendOneRPL(RPL_TOPIC(user.getNickname(), channelName, getTopic()), fd);
	sendRPLtoChan(RPL_NAMREPLY(newnick, channelName, getListUsers()));
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
