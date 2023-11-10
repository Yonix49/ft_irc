/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 11:34:55 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/10 11:36:54 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void	Server::mode(std::string param, int fd)
{
	Server &server = Server::getInstance();
	User *user = server.getUserNo(fd);
	std::vector<std::string> cmdLine = server.get_vector_ref(param);

	int	i = 0;
	if (cmdLine.size() > 4)
		return ; // rien envoyer juste ne pas executer la commande 
	if (cmdLine.size() < 3)
	{
		sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "MODE"), fd);
		return ;
	}
	i = server.channelExist(cmdLine[1]);
	if (i == -1)
	{
		sendOneRPL(ERR_NOSUCHCHANNEL((*user).getNickname(), cmdLine[1]), fd);
		// std::cout << "RETURN -2" << std::endl;
		return ;
	}
	if (server._channels[i]->isUserinchan(user->getNickname(), 0) == -1)
	{
		sendOneRPL(ERR_NOTONCHANNEL(user->getNickname(), cmdLine[1]), fd);
		return ;
	}
	if (user->getisOperator() > 0)
	{
		if (cmdLine.size() > 2 && cmdLine[2][1] == 'l')
			server.mode_l(cmdLine, i, user);
		else if (cmdLine.size() > 2 && cmdLine[2][1] == 'k')
			server.mode_k(cmdLine, i, fd, user);
		else if (cmdLine.size() == 3 && cmdLine[2][1] == 'i')
			server.mode_i(cmdLine, i);
		else if (cmdLine.size() == 3 && cmdLine[2][1] == 't')
			server.mode_t(cmdLine, i, user);
		else if (cmdLine.size() == 4 && cmdLine[2][1] == 'o')
			server.mode_o(cmdLine, i, fd, user);
	}
	else
	{
		//! faire cette condition autrement sinon ca marche pas ( peut etre directement dans les fonctions)
		sendOneRPL(ERR_CHANOPRIVSNEED(user->getNickname(), cmdLine[1]), fd);
		std::cerr << "MODE error" << std::endl;
	}
}




// au moins mode i et mode t ne sont pas encore bon avec genre +i et -i



int		Server::mode_o(std::vector<std::string> cmdLine, int i, int fd, User *user)
{
	User *target = getUserString(cmdLine[3]);
	std::string msgToSend;
	if (target == NULL)
	{
		sendOneRPL(ERR_NOSUCHNICK(user->getNickname(), cmdLine[3]), fd);
		return (-1);
	}
	if (_channels[i]->isUserinchan(target->getNickname(), 0) == -1)
	{
		sendOneRPL(ERR_USERNOTINCHANNEL(user->getNickname(), target->getNickname(), cmdLine[1]), fd);
		return -2;
	}
	if (cmdLine[2][0] == '+')
	{
		if (target->getisOperator() < 1)
		{
			target->setIsOperator(1);
			_channels[i]->addOperator(*target);
			msgToSend += '+';
		}
		else
		{
			sendOneRPL(ERR_CHANFPRIVSNEED(user->getNickname(), cmdLine[1]), fd);
			return -1;
		}
	}
	else if (cmdLine[2][0] == '-')
	{
		if (target->getisOperator() < 2)
		{
			target->setIsOperator(0);
			_channels[i]->rmOperator(*target);
			msgToSend += '-';
		}
		else
		{
			sendOneRPL(ERR_CHANFPRIVSNEED(user->getNickname(), cmdLine[1]), fd);
			return -1;
		}
	}
	msgToSend += cmdLine[2][1];
	_channels[i]->sendRPLtoChan(MODE_CHANNEL(user->getNickname(), user->getUsername(), cmdLine[1], msgToSend));
	return 0;
}

int		Server::mode_l(std::vector<std::string> cmdLine, int i, User *user)
{
	std::string msgToSend;
	if (_channels[i]->getMode_i() == false && cmdLine.size() == 4 && cmdLine[2][0] == '+')
	{
		msgToSend += '+';
		int	limit = atoi(cmdLine[3].c_str()); 
		if (limit > 0 && limit < 2147483647)
			_channels[i]->setLimitUsers(limit);
		_channels[i]->setMode_l(true);
		std::cout << "LIMITUSER SET TO: " << _channels[i]->getLimitUsers() << std::endl;
	}
	else if (_channels[i]->getMode_l() == true && cmdLine[2][0] == '-')
	{	
		_channels[i]->setLimitUsers(0);
		_channels[i]->setMode_l(false);
		msgToSend += '-';
	}
	msgToSend += cmdLine[2][1];
	_channels[i]->sendRPLtoChan(MODE_CHANNEL(user->getNickname(), user->getUsername(), cmdLine[1], msgToSend));
	return (0);
}

int		Server::mode_t(std::vector<std::string> cmdLine, int i, User *user)
{
	std::string msgToSend;
	if (_channels[i]->getMode_t() == false && cmdLine[2][0] == '+')
	{
		_channels[i]->setMode_t(true);
		msgToSend += '+';
	}
	else if (cmdLine[2][0] == '-')
	{
		_channels[i]->setMode_t(false);
		msgToSend += '-';
	}
	msgToSend += cmdLine[2][1];
	_channels[i]->sendRPLtoChan(MODE_CHANNEL(user->getNickname(), user->getUsername()	, cmdLine[1], msgToSend));
	return (0);
}

int		Server::mode_k(std::vector<std::string> cmdLine, int i, int fd, User *user)
{
	if (_channels[i]->getMode_k() == false && cmdLine.size() == 4 && cmdLine[2][0] == '+')
	{
		if (cmdLine[3].find(' ') == std::string::npos)
		{
			_channels[i]->setPassword(cmdLine[3]);
			_channels[i]->setMode_k(true);
			std::string msgToSend("+k");
			_channels[i]->sendRPLtoChan(MODE_CHANNEL(user->getNickname(), user->getUsername()	, cmdLine[1], msgToSend));
		}
		else
		{
			std::cout << "FAILED TO SET PASSWORD: " << _channels[i]->getLimitUsers() << std::endl;
			sendOneRPL(ERR_INVALIDMODEPARAM(user->getNickname(), cmdLine[1], cmdLine[2], cmdLine[3]), fd);
			return ( -3);
		}
	}
	else if (_channels[i]->getMode_k() == true && cmdLine[2][0] == '-')
	{
		_channels[i]->getPassword().clear();
		_channels[i]->setMode_k(false);
		std::string msgToSend("-k");
		_channels[i]->sendRPLtoChan(MODE_CHANNEL(user->getNickname(), user->getUsername()	, cmdLine[1], msgToSend));
	}
	return (0);
}

int		Server::mode_i(std::vector<std::string> cmdLine, int i)
{
	if (_channels[i]->getMode_i() == false && cmdLine[2][0] == '+')
		_channels[i]->setMode_i(true);
	else if (cmdLine[2][0] == '-')
		_channels[i]->setMode_i(false);
	return (0);
}
