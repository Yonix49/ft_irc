/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 16:35:21 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/10 15:37:00 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"




void	Server::quit(std::string param, int fd)
{
	Server &server = Server::getInstance();
	User *user = server.getUserNo(fd);
	std::vector<std::string> cmdLine = server.get_vector_ref(param);
	std::string reason;
	std::string partStr;

	user->setHasQuit(1);
	if (cmdLine.size() > 1)
		reason = " " + cmdLine[1];
	else
		reason = " leaving";

	std::string chans;

	partStr = "PART " + server.getAllChanForQuit(&server, user->getNickname()) + reason;

	server.part(partStr, fd);
	server.updateInvitedListAfterQuit(server, user->getNickname());

	// supprimer le user du vector _users dans le serveur
	for (std::vector<User>::iterator it = server._users.begin(); it < server._users.end(); ++it)
	{
		if ((*it).getFd() == user->getFd())
		{
			server._users.erase(it);
			break ;
		}
	}	
	
	// retirer du epoll
	if (epoll_ctl(server._serv.epollFd, EPOLL_CTL_DEL, fd, &(server._serv.event)) == -1)
    	perror("Erreur lors de la suppression du descripteur de fichier de l'événement epoll");
}

std::string	Server::getAllChanForQuit(Server *server, std::string nickname)
{
	std::string chans;

	for (std::vector<Channel *>::iterator it = server->_channels.begin(); it != server->_channels.end(); ++it)
	{
		if ((*it)->isUserinchan(nickname, 0) > -1)
		{
			chans = chans + (*it)->getName();
			if (it + 1 < server->_channels.end())
				chans = chans + ",";
		}
	}
	return (chans);
}

void	Server::updateInvitedListAfterQuit(Server server, std::string nickname)
{
	std::string chans;
	for (std::vector<Channel *>::iterator it = server._channels.begin(); it != server._channels.end(); ++it)
	{
		(*it)->updateInvited(nickname);
	}
}
