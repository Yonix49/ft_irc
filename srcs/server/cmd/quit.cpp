/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 16:35:21 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/10 12:37:32 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"




void	Server::quit(std::string param, int fd)
{
	std::cout << "APPEL DE QUIT " << std::endl;

	// faire quitter le user de tout les channels, de tout les vecteurs etc check dans les_bails.txt
	Server &server = Server::getInstance(); // Obtenez une référence à l'instance unique de la classe
	User *user = server.getUserNo(fd);
	std::cout << "PARAM = " << param << std::endl;
	std::vector<std::string> cmdLine = server.get_vector_ref(param);
	std::string reason;
	std::string partStr;

	user->setHasQuit(1);
	for ( size_t i = 0 ; i < cmdLine.size(); i++)
		std::cout << "cmdLine[" << i << "] = " << cmdLine[i] << std::endl;
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
		std::cout << "Je suis ici " << std::endl;
		if ((*it)->isUserinchan(nickname, 0) > -1)
		{
			chans = chans + (*it)->getName();
			if (it + 1 < server->_channels.end())
			{
				chans = chans + ",";
			}
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
