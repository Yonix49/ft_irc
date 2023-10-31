/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 14:05:02 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/31 11:34:24 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int	Server::invite(std::vector<std::string> cmdLine, int fd)
{
	User * user = getUserNo(fd);
	if (cmdLine.size() != 3)
	{
		sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), cmdLine[0]), fd);
		return (-1);
	}

	int	i = channelExist(cmdLine[2]);

	// check si le user a inviter existe ou pas, sinon envoyer ERR_nosuchuser
	if (findUser(cmdLine[1], _users) == -1)
	{
		// no user
		std::cerr << "INVITE RETURN -2" << std::endl;
		return (-2);
	}
	
	// check si le channel existe, si non envoyer ERR_NOSUCHCHANNEL
	if (i < 0)
	{
		sendOneRPL(ERR_NOSUCHCHANNEL(user->getNickname(), cmdLine[2]), fd);
		return (-3);
	}
	
	// check si le user qui invite est bien dans le channel ERR_NOTONCHANNEL
	if (_channels[i]->isUserinchan(user->getNickname(), 0) == -1)
	{
		sendOneRPL(ERR_NOTONCHANNEL(user->getNickname(), cmdLine[2]), fd);
		return (-4);
	}
	
	// si le channel est en mode invite only, seul les operateurs peuvent inviter, ERR_CHANOPRIVSNEEDED
	if (_channels[i]->getMode_i() == true && _channels[i]->isUserinchan(user->getNickname(), 1) == -1)
	{
		sendOneRPL(ERR_CHANOPRIVSNEED(user->getNickname(), cmdLine[2]), fd);
		return (-5);
	}
	
	// le user qui est invite ne doit pas deja etre dans les channel, si oui envoyer  ERR_USERONCHANNEL
	if (_channels[i]->isUserinchan(cmdLine[1], 0) > -1)
	{
		std::string irssi("IRSSI");
		sendOneRPL(ERR_USERONCHANNEL(irssi, cmdLine[1], cmdLine[2]), fd);
		return (-6);
	}
	
	// si tout est ok, on ajoute le user dans la liste des invites et on envoie les rpl necessaires
	User *target = getUserString(cmdLine[1]);
	
	_channels[i]->addInvitedUser(target->getNickname());
	target->addisInvited(_channels[i]->getName());
	sendOneRPL(RPL_INVITING(user->getNickname(), target->getNickname(), cmdLine[2]), fd);
	sendOneRPL(NOCTICE_CLIENT_INVITE(user->getNickname(), target->getNickname()), target->getFd());
	return (0);
}
