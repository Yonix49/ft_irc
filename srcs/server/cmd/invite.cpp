/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 14:05:02 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/10 14:46:45 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"

void	Server::invite(std::string param, int fd)
{
	Server &server = Server::getInstance();
	User *user = server.getUserNo(fd);
	std::vector<std::string> cmdLine = server.get_vector_ref(param);

	if (cmdLine.size() != 3)
	{
		sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), cmdLine[0]), fd);
		return ;
	}

	int	i = server.channelExist(cmdLine[2]);

	// check si le user a inviter existe ou pas, sinon envoyer ERR_nosuchuser
	if (findUser(cmdLine[1], server._users) == -1)
	{
		// no user
		sendOneRPL(ERR_NOSUCHNICK(user->getNickname(), cmdLine[2]), fd);
		return ;
	}
	
	// check si le channel existe, si non envoyer ERR_NOSUCHCHANNEL
	if (i < 0)
	{
		sendOneRPL(ERR_NOSUCHCHANNEL(user->getNickname(), cmdLine[2]), fd);
		return ;
	}
	
	// check si le user qui invite est bien dans le channel ERR_NOTONCHANNEL
	if (server._channels[i]->isUserinchan(user->getNickname(), 0) == -1)
	{
		sendOneRPL(ERR_NOTONCHANNEL(user->getNickname(), cmdLine[2]), fd);
		return ;
	}
	
	// si le channel est en mode invite only, seul les operateurs peuvent inviter, ERR_CHANOPRIVSNEEDED
	if (server._channels[i]->getMode_i() == true && server._channels[i]->isUserinchan(user->getNickname(), 1) == -1)
	{
		sendOneRPL(ERR_CHANOPRIVSNEED(user->getNickname(), cmdLine[2]), fd);
		return ;
	}
	
	// le user qui est invite ne doit pas deja etre dans les channel, si oui envoyer  ERR_USERONCHANNEL
	if (server._channels[i]->isUserinchan(cmdLine[1], 0) > -1)
	{
		sendOneRPL(ERR_USERONCHANNEL(user->getNickname(), cmdLine[1], cmdLine[2]), fd);
		return ;
	}
	
	// si tout est ok, on ajoute le user dans la liste des invites et on envoie les rpl necessaires
	User *target = server.getUserString(cmdLine[1]);
	
	server._channels[i]->addInvitedUser(target->getNickname());
	target->addisInvited(server._channels[i]->getName());
	sendOneRPL(RPL_INVITING(user->getNickname(), target->getNickname(), cmdLine[2]), fd);
	sendOneRPL(NOCTICE_CLIENT_INVITE(user->getNickname(), target->getNickname()), target->getFd());
	return ;
}
