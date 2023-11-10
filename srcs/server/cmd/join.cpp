/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:22:57 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/10 12:36:54 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"




// ne pas oublier de gerer si un channel est supprime ou pas
// si oui, il faut enlever le channel du vector is_invited sur les users invites

// bien gerer la deconection, si un user est inviter et quitte le serveur, il faut le retirer
// de la liste des invites au channel (ca peut causer des soucis en cas de reconnection sur le meme client)




void Server::join(std::string param, int fd)
{
	Server &server = Server::getInstance(); // Obtenez une référence à l'instance unique de la classe
	User *user = server.getUserNo(fd);
	std::vector<std::string> cmdLine = server.get_vector_ref(param);
	std::cout << "COMMAND JOIN IS CALLED" << std::endl;
	std::string	newnick;

	if (cmdLine.size() < 2)
	{
		sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "JOIN"), fd);
		return ;
	}

	std::string	channelName = getChannelName(cmdLine[1]);
	if (channelName[0] == ',')
	{
		sendOneRPL(ERR_BADCHANMASK(cmdLine[1]), fd);
		return ;
	}

	int	i = server.channelExist(channelName);
	if (i == -1)
	{
		Channel *NewChannel = new Channel(channelName);
		
		user->setIsOperator(2);
		if (NewChannel->addUser(*user, 2, channelName, fd) == -1)
			return ;
		NewChannel->setNbUsers(1);
		if (cmdLine.size() > 2)
			NewChannel->setTopic(cmdLine[2]);
		server._channels.push_back(NewChannel);
	}
	else	
	{
		if (server._channels[i]->getLimitUsers() > 0 && server._channels[i]->getNbUsers() >= server._channels[i]->getLimitUsers())
		{
			sendOneRPL(ERR_CHANNELISFULL(user->getNickname(), channelName), fd);
			return ;
		}
		else if (server._channels[i]->getMode_i() == true && user->getisInvited(channelName) == 0)
		{
			sendOneRPL(ERR_INVITEONLYCHAN(user->getNickname(), channelName), fd);
			return ;
		}
		else if (server._channels[i]->getMode_k() == true)
		{
			if (cmdLine.size() != 3 || cmdLine[2].compare(server._channels[i]->getPassword()))
			{
				sendOneRPL(ERR_BADCHANNELKEY(user->getNickname(), server._channels[i]->getName()), fd);
				return ;
			}
		}
		user->setIsOperator(0);
		if (server._channels[i]->addUser(*user, 0, channelName, fd) == -1)
			return ;
	}
	return ;
}

int Server::channelExist(std::string channelName)
{
	int i = 0;

	for (std::vector<Channel *>::iterator it = _channels.begin() ; it != _channels.end() ; ++it)
	{
		if ((*it)->getName() == channelName)
			return (i);
		i++;
	}
	return (-1);
}