/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:22:57 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/03 18:59:54 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


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
	// char		option;
	if (cmdLine.size() < 2)
	{
		// rpl needmoreparams
		return ;
	}
	std::string	channelName = getChannelName(cmdLine[1]);
	if (channelName.c_str() == NULL)
	{
		return ;
	}
	int			i = server.channelExist(channelName);

	if (i == -1)
	{
		Channel *NewChannel = new Channel(channelName);
		
		user->setIsOperator(2);
		if (NewChannel->addUser(*user, 2, channelName, fd) == -1)
		{
			// std::cout << "RETURN -3" << std::endl;
			return ;
		}
		NewChannel->setNbUsers(1);
		// std::cout << "NB USER IN CHANNEl " << NewChannel->getName() << " IS " << NewChannel->getNbUsers() << std::endl;
		server._channels.push_back(NewChannel);
		//! ne pas oublier de prendre le troisieme argument si il y en a un et le mettre dans topic.
		
		
		// applyOptions(cmdLine)->si le channel est creer avec une option, appeler MODE
		// david l'a pas fait donc jsp si on doit le faire ou aps
		// std::cout << "channel " << channelName << " is created and " << (*user).getNickname() << " joined the channel"  << std::endl; 
	}
	else if (cmdLine.size() == 2)
	{
		if (server._channels[i]->getLimitUsers() > 0 && server._channels[i]->getNbUsers() >= server._channels[i]->getLimitUsers())
		{
			sendOneRPL(ERR_CHANNELISFULL(user->getNickname(), channelName), fd);
			// std::cout << "RETURN -4" << std::endl;
			return ;
		}
		else if (server._channels[i]->getMode_i() == true && user->getisInvited(channelName) == 0)
		{
			// std::cout << "RETURN -5" << std::endl;
			sendOneRPL(ERR_INVITEONLYCHAN(user->getNickname(), channelName), fd);
			return ;
		}
		else if (server._channels[i]->getPassword().empty() == false
				&& (cmdLine.size() != 3 || cmdLine[2].compare(server._channels[i]->getPassword())))
		{
			// std::cout << "RETURN -6" << std::endl;
			sendOneRPL(ERR_BADCHANNELKEY(user->getNickname(), server._channels[i]->getName()), fd);
			return ;
		}
		user->setIsOperator(0);
		if (server._channels[i]->addUser(*user, 0, channelName, fd) == -1)
		{
			// std::cout << "RETURN -3 (2)" << std::endl;
			return ;
		}
		// std::cout << "NB USER IN CHANNEl " << server._channels[i]->getName() << " IS " << server._channels[i]->getNbUsers() << std::endl;
		// std::cout << "user : " << (*user).getNickname() << " joined the channel : " << channelName << std::endl; 
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