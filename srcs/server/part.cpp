/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 16:11:30 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/03 18:50:52 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "server.hpp"

void	Server::part(std::string param, int fd)
{
	Server &server = Server::getInstance(); // Obtenez une référence à l'instance unique de la classe
	User *user = server.getUserNo(fd);
	std::vector<std::string> cmdLine = server.get_vector_ref(param);
	
	if (cmdLine.size() < 2)
	{
		sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "PART"), fd);
		return ;
	}
	
	std::string reason("");
	if (cmdLine.size() > 2)
		reason = cmdLine[2];
	int j = 0;
	std::vector<std::string> chans = split(cmdLine[1], ",");
	for (std::vector<std::string>::iterator it = chans.begin(); it != chans.end(); it++)
	{
		// check si le channel existe
		int i = server.channelExist(*it);
		if (i == -1)
		{
			sendOneRPL(ERR_NOSUCHCHANNEL(user->getNickname(), chans[j]), fd);
			j++;
			continue ;
		}
		// check si le user est pas dans le channel
		if (user->getHasQuit() == 0 && server._channels[i]->isUserinchan(user->getNickname(), 0) == -1)
		{
			sendOneRPL(ERR_NOTONCHANNEL(user->getNickname(), chans[j]), fd);
			j++;
			continue ;
		}
		// faire quitter le channel
		chans[j].erase(0, 1);
		if (user->getHasQuit() == 0)
			server._channels[i]->sendRPLtoChan(PART(user->getNickname(), user->getUsername(), chans[j], reason));
		else
			server._channels[i]->sendRPLtoChan(QUIT(user->getNickname(), user->getUsername(), reason));
		if (user->getisOperator() > 0)
			server._channels[i]->rmOperator(*user);
		server._channels[i]->rmUser(*user);
		server._channels[i]->setNbUsers(server._channels[i]->getNbUsers() - 1);
		std::cout << "ON SUPPRIME LE USER IICIIIIIIIII DU CHAN SUIVANT :" << server._channels[i]->getName() << std::endl;

		// si il n'y a plus de user dans le channel, supprimer le channel
		if (server._channels[i]->getNbUsers() == 0)
			server.rmChannel(*server._channels[i]);
		j++;
	}
}

