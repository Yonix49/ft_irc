/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:13:50 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/07 11:22:13 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void	Server::kick(std::string param, int fd)
{
	(void)param;
	(void)fd;
	Server &server = Server::getInstance();
	User *user = server.getUserNo(fd);
	std::vector<std::string> cmdLine = server.get_vector_ref(param);

	if (cmdLine.size() < 3)
	{
		sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "PART"), fd);
		return ;
	}
	int i = server.channelExist(cmdLine[1]);
	if (i == -1)
	{
		sendOneRPL(ERR_NOSUCHCHANNEL(user->getNickname(), cmdLine[1]), fd);
		return ;
	}
	if (server._channels[i]->isUserinchan(user->getNickname(), 0) == -1)
	{
		sendOneRPL(ERR_NOTONCHANNEL(user->getNickname(), cmdLine[1]), fd);
		return ;
	}

	User *target = server.getUserString(cmdLine[2]);
	if (target == NULL)
	{
		sendOneRPL(ERR_NOSUCHNICK(user->getNickname(), cmdLine[2]), fd);
		return ;
	}
	if (server._channels[i]->isUserinchan(target->getNickname(), 0) == -1)
	{
		sendOneRPL(ERR_USERNOTINCHANNEL(user->getNickname(), target->getNickname(), cmdLine[1]), fd);
		return ;
	}
	if (user->getisOperator() < 1 )
	{
		sendOneRPL(ERR_CHANOPRIVSNEEDED(cmdLine[1], user->getNickname()), fd);
		return ;
	}
	if (user->getisOperator() == 1 && target->getisOperator() > 0)
	{
		sendOneRPL(ERR_CHANFPRIVSNEED(user->getNickname(), cmdLine[1]), fd);	
		return ;
	}
	std::string reason("");
	if (cmdLine.size() > 3)
	{
		for (size_t i = 3; i < cmdLine.size(); i++)
		{
			reason = reason + cmdLine[i];
			if (i + 1 < cmdLine.size())
				reason = reason + " ";
		}
	}
	server._channels[i]->sendRPLtoChan(KICK(user->getNickname(), user->getUsername(),
										cmdLine[1], target->getNickname(), reason));
	if (user->getisOperator() > 0)
		server._channels[i]->rmOperator(*target);
	server._channels[i]->rmUser(*target);
	server._channels[i]->setNbUsers(server._channels[i]->getNbUsers() - 1);
}
