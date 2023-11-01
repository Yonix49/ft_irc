/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 11:34:55 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/01 17:46:05 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void	Server::mode(std::string param, int fd)
{
	Server &server = Server::getInstance(); // Obtenez une référence à l'instance unique de la classe
	User *user = server.getUserNo(fd);
	std::vector<std::string> cmdLine = server.get_vector_ref(param);

	int	i = 0;

	// std::cout << "COMMAND MODE IS CALLED" << std::endl;
	// std::cout << "cmdLine.size() = " << cmdLine.size() << std::endl;
	// for (std::vector<std::string>::iterator i = cmdLine.begin(); i != cmdLine.end(); i++)
		// std::cout << "cmdLine[] = " << i->c_str() << std::endl;

	if (cmdLine.size() > 4)
	{
		std::cout << "RETURN -1" << std::endl;	
		return ; // rien envoyer juste ne pas executer la commande 
	}
	i = server.channelExist(cmdLine[1]);
	if (i == -1)
	{
		sendOneRPL(ERR_NOSUCHCHANNEL((*user).getNickname(), cmdLine[1]), fd);
		std::cout << "RETURN -2" << std::endl;	
		return ;
	}
	// if (cmdLine.size() == 2)
	// {
	// 	std::string modes = server._channels[i]->getModes();
	// 	sendOneRPL(RPL_CHANNELMODEIS((*user).getNickname(), cmdLine[1], modes), fd);
	// }
	if (user->getisOperator() > 0)
	{
		if (cmdLine.size() > 2 && cmdLine[2][1] == 'l')
			server.mode_l(cmdLine, i);
		else if (cmdLine.size() > 2 && cmdLine[2][1] == 'k')
			server.mode_k(cmdLine, i, fd, user);
		else if (cmdLine.size() == 3 && cmdLine[2][1] == 'i')
			server.mode_i(cmdLine, i);
		else if (cmdLine.size() == 3 && cmdLine[2][1] == 't')
			server.mode_t(cmdLine, i);
	}
	else
		std::cout << "NOT OPERATOR (MODE): " << server._channels[i]->getLimitUsers() << std::endl;

	// il reste mode +t, +o a faire et tout les modes dans leurs fonctions proprement
	// ne pas oublier de faire les modes dans l'autre sens (+l pour set et UNSET)
	return ;
}




// au moins mode i et mode t ne sont pas encore bon avec genre +i et -i





int		Server::mode_l(std::vector<std::string> cmdLine, int i)
{
	if (_channels[i]->getMode_i() == false && cmdLine.size() == 4 && cmdLine[2][0] == '+')
	{
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
	}
	return (0);
}

int		Server::mode_t(std::vector<std::string> cmdLine, int i)
{
	if (_channels[i]->getMode_t() == false && cmdLine[2][1] == '+')
		_channels[i]->setMode_t(true);
	else if (cmdLine[2][1] == '-')
		_channels[i]->setMode_t(false);
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
	}
	return (0);
}

int		Server::mode_i(std::vector<std::string> cmdLine, int i)
{
	std::cout << "passage par le mode" << std::endl;
	if (_channels[i]->getMode_i() == false && cmdLine[2][1] == '+')
		_channels[i]->setMode_i(true);
	else if (cmdLine[2][1] == '-')
		_channels[i]->setMode_i(false);
	return (0);
}
