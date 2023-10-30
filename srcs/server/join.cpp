/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:22:57 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/30 16:26:38 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int	Server::join(std::vector<std::string> cmdLine, int fd)
{
	std::cout << "COMMAND JOIN IS CALLED" << std::endl;
	std::string	newnick;
	// char		option;
	if (cmdLine.size() < 2 || cmdLine.size() > 3)
	{
		std::cout << "RETURN -1" << std::endl;
		// rpl nombre d'arguments pas bon
		return (-1);
	}
	std::string	channelName = getChannelName(cmdLine[1]);
	std::cout << "channelName = " << channelName << std::endl;
	if (channelName.c_str() == NULL)
	{
		std::cout << "RETURN -2" << std::endl;
		return (-2);
	}
	User		*user = getUserNo(fd);
	int			i = channelExist(channelName);

	if (i == -1)
	{
		Channel *NewChannel = new Channel(channelName);
		
		// applyOptions(cmdLine)->si le channel est creer avec une option, appeler MODE
		user->setIsOperator(2);
		if (NewChannel->addUser(*user, 2, channelName, fd) == -1)
		{
			std::cout << "RETURN -3" << std::endl;
			return (-3);
		}
		NewChannel->setNbUsers(1);
		std::cout << "NB USER IN CHANNEl " << NewChannel->getName() << " IS " << NewChannel->getNbUsers() << std::endl;
		_channels.push_back(NewChannel);
		std::cout << "channel " << channelName << " is created and " << (*user).getNickname() << " joined the channel"  << std::endl; 
	}
	else
	{
		// std::cout << "I = [" << i << "]" << std::endl;
		// on verifie si il y a le mode invit-only, si oui, on check si le user est invite

		
		
		// si tout est valide, on ajoute le user a la chan
		if (_channels[i]->getLimitUsers() > 0 && _channels[i]->getNbUsers() >= _channels[i]->getLimitUsers())
		{
			sendOneRPL(ERR_CHANNELISFULL(user->getNickname(), channelName), fd);
			std::cout << "RETURN -4" << std::endl;
			return (-4);
		}
		else if (_channels[i]->getIsInviteOnlyMod() == true && user->getisInvited(channelName) == 0)
		{
			std::cout << "RETURN -5" << std::endl;
			sendOneRPL(ERR_INVITEONLYCHAN(user->getNickname(), channelName), fd);
			return (-5);
		}
		else if (_channels[i]->getPassword().empty() == false
				&& (cmdLine.size() != 3 || cmdLine[2].compare(_channels[i]->getPassword())))
		{
			std::cout << "RETURN -6" << std::endl;
			sendOneRPL(ERR_BADCHANNELKEY(user->getNickname(), _channels[i]->getName()), fd);
			return (-6);
		}
		user->setIsOperator(0);
		if (_channels[i]->addUser(*user, 0, channelName, fd) == -1)
		{
			std::cout << "RETURN -3 (2)" << std::endl;
			return (-3);
		}
		std::cout << "NB USER IN CHANNEl " << _channels[i]->getName() << " IS " << _channels[i]->getNbUsers() << std::endl;
		// _channels[i]->setNbUsers(_channels[i]->getNbUsers() + 1);
		std::cout << "user : " << (*user).getNickname() << " joined the channel : " << channelName << std::endl; 
	}
	std::cout << "AFTER JOIN =========== users in list = " << _channels[0]->getListUsers() << std::endl;
	return (0);
}


// ne pas oublier de gerer si un channel est supprime ou pas
// si oui, il faut enlever le channel du vector is_invited sur les users invites

// bien gerer la deconection, si un user est inviter et quitte le serveur, il faut le retirer
// de la liste des invites au channel (ca peut causer des soucis en cas de reconnection sur le meme client)



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