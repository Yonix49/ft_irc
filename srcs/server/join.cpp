/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:22:57 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/31 17:06:12 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


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