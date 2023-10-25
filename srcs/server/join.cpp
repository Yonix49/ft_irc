/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:22:57 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/25 17:19:28 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int	Server::join(std::vector<std::string> cmdLine, int fd)
{
	// check le nb d'arguments ici

	// verifier si le channel a join existe ou pas
	User	*user = getUserNo(fd);
	int		i = channelExist(cmdLine[1]);
	if (i == -1)
	{
		// si non, on creer un channel et on y ajoute le user en mode operator
		Channel NewChannel(cmdLine[1]);
		NewChannel.addUser(*user);
		_channels.push_back(NewChannel);
		// send(fd, RPL_TOPIC(user->getNickname(), cmdLine[1], NewChannel.getTopic()), );
		std::cout << "channel " << cmdLine[1] << " is created and " << (*user).getNickname() << " joined the channel"  << std::endl; 
	}
	else
	{
		// si oui, on ajoute le user dans le channel en mode user basique
		_channels[i].addUser(*user);
		std::cout << "user : " << (*user).getNickname() << " joined the channel : " << cmdLine[1] << std::endl; 
	}
	return (0);
}


int Server::channelExist(std::string channelName)
{
	int i = 0;

	for (std::vector<Channel>::iterator it = _channels.begin() ; it != _channels.end() ; ++it)
	{
		if (it->getName() == channelName)
			return (i);
		i++;
	}
	return (-1);
}