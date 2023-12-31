/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:22:57 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/11 15:05:39 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"

void	Server::print(int fd)
{
	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		std::cout << "======== " << it->getNickname() << std::endl;
		std::cout << "======== " << it->getFd() << std::endl;
		std::cout << "======== " << &it << std::endl;
		std::cout << "=================================================	 " << std::endl;
		if (it->getFd() == fd)
			std::cout << "FD FOUUUUUUND" << std::endl;
	}
}

void Server::join(std::string param, int fd)
{
	Server &server = Server::getInstance();
	User *user = server.getUserNo(fd);
	(void)(user);
	std::vector<std::string> cmdLine = server.get_vector_ref(param);
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
