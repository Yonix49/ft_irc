/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:46:17 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/07 13:33:53 by kgezgin          ###   ########.fr       */
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

int	&Channel::getNbUsers(void)
{
	return (_nbUsers);
}

std::string		Channel::getModes()
{
	std::string res;

	if (getMode_i() == true)
		res = res + " +i";
	if (getMode_t() == true)
		res = res + " +t";
	if (getMode_k() == true)
		res = res + " +k";
	if (getMode_l() == true)
		res = res + " +l";
	return (res);
}

std::string		Channel::getListUsers(void)
{
	std::string userInfo;
	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		// if (it->getInvisible() == false)
		// {
			if (it->getisOperator() > 0)
				userInfo += '@';
			else
				userInfo += '+';
			userInfo += it->getNickname();
			userInfo += ' ';
		// }
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

std::vector<User> &Channel::getUsers(void)
{
	return (_users);
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
	std::string newnick;
	// std::cout << "ADDUSER IS CALLED" << std::endl;
	for (std::vector<User>::iterator it = _users.begin(); it < _users.end(); ++it)
	{
		if (it->getNickname() == user.getNickname())
		{
			// rpl user already in channel
			return (-1);
			std::cerr << "RETURN IN ADUSER" << std::endl;
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
	channelName.erase(0, 1);
	
	std::cout << "ICI" << std::endl;
	std::cout << JOIN(user.getNickname(), user.getUsername(), channelName) << std::endl;
	std::cout << "ICI" << std::endl;
	
	sendRPLtoChan(JOIN(user.getNickname(), user.getUsername(), channelName));
	_users.push_back(user);
	_nbUsers = _nbUsers + 1;
	if (getTopic().empty() == true)
		sendOneRPL(RPL_NOTOPIC(user.getNickname(), channelName), fd);
	else
		sendOneRPL(RPL_TOPIC(user.getNickname(), channelName, _topic), fd);
	// std::cout << "list Users = " << getListUsers() << std::endl;
	std::cout << "NEWNICKNEWNICKNEWNICKNEWNICKNEWNICKNEWNICKNEWNICKNEWNICK = " << newnick << std::endl;
	std::cout << "CHANNELCHANNELCHANNELCHANNELCHANNELCHANNELCHANNELCHANNEL = " << channelName << std::endl;
	sendOneRPL(RPL_NAMREPLY(newnick, channelName, getListUsers()), fd);
	sendOneRPL(RPL_ENDOFNAMES(newnick, channelName), fd);
	return (0);
}

void	Channel::addOperator(User user)
{
	_operators.push_back(user);
}


void	Channel::rmOperator(User user)
{
	for (std::vector<User>::iterator it = _operators.begin(); it < _operators.end(); ++it)
	{
		if (user.getFd() == it->getFd())
		{
			_operators.erase(it);
			break ;
		}
	}
}

void	Channel::rmUser(User user)
{
	for (std::vector<User>::iterator it = _users.begin(); it < _users.end(); ++it)
	{
		if (user.getFd() == it->getFd())
		{
			_users.erase(it);
			break ;
		}
	}
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
	{
		sendOneRPL(rpl, _users[i].getFd());
		// std::cout << "msg send to " << _users[i].getFd() << std::endl;
	}
}



void	Channel::sendMSGtoChan(std::string rpl, int fd)
{

	for (unsigned long i = 0; i < _users.size(); i++)
	{
		if (_users[i].getFd() != fd)
			sendOneRPL(rpl, _users[i].getFd());
	}
}
