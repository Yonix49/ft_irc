/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 11:34:55 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/30 17:21:41 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int		Server::mode(std::vector<std::string> cmdLine, int fd)
{
	User *user;
	
	user = getUserNo(fd);
	int	i = 0;

	std::cout << "COMMAND MODE IS CALLED" << std::endl;
	// std::cout << "user -> " << user->getNickname() << " is = " << user->getisOperator() << std::endl;
	std::cout << "cmdLine.size() = " << cmdLine.size() << std::endl;
	for (std::vector<std::string>::iterator i = cmdLine.begin(); i != cmdLine.end(); i++)
		std::cout << "cmdLine[] = " << i->c_str() << std::endl;
		
	if (cmdLine.size() > 4)
	{
		std::cout << "RETURN -1" << std::endl;	
		return (-1); // rien envoyer juste ne pas executer la commande 
	}
	i = channelExist(cmdLine[1]);
	if (i == -1)
	{
		sendOneRPL(ERR_NOSUCHCHANNEL((*user).getNickname(), cmdLine[1]), fd);
		std::cout << "RETURN -2" << std::endl;	
		return (-2);
	}
	if (user->getisOperator()> 0)
	{
		if (cmdLine.size() == 4 && !cmdLine[2].compare("+l"))
		{
			int	limit = atoi(cmdLine[3].c_str()); 
			if (limit > 0 && limit < 2147483647)
				_channels[i]->setLimitUsers(limit);
			std::cout << "LIMITUSER SET TO: " << _channels[i]->getLimitUsers() << std::endl;
		}
		else if (cmdLine.size() == 4 && !cmdLine[2].compare("+k"))
		{
			if (cmdLine[3].find(' ') == std::string::npos)
				_channels[i]->setPassword(cmdLine[3]);
			else
			{
				std::cout << "FAILED TO SET PASSWORD: " << _channels[i]->getLimitUsers() << std::endl;
				sendOneRPL(ERR_INVALIDMODEPARAM(user->getNickname(), cmdLine[1], cmdLine[2], cmdLine[3]), fd);
				return ( -3);
			}
		}
		else if (cmdLine.size() == 3 && !cmdLine[2].compare("+i"))
			_channels[i]->setIsInviteOnlyMod(true);
	}
	else
		std::cout << "FAILED TO SET LIMITUSER: " << _channels[i]->getLimitUsers() << std::endl;

	// il reste mode +t, +o a faire et tout les modes dans leurs fonctions proprement
	// ne pas oublier de faire les modes dans l'autre sens (+l pour set et UNSET)

	return (0);
}
