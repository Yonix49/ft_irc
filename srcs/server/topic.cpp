/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 13:07:11 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/01 16:01:58 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include"server.hpp"

void	Server::topic(std::string param, int fd)
{
	Server &server = Server::getInstance(); // Obtenez une référence à l'instance unique de la classe
	User *user = server.getUserNo(fd);
	std::vector<std::string> cmdLine = server.get_vector_ref(param);

	if (cmdLine.size() == 1)
		return ; // need more params
	int i = server.channelExist(cmdLine[1]);
	if (i < 0)
		return ; // no such channel
	if (server._channels[i]->isUserinchan(user->getNickname(), 0) == -1)
	{
		// check si le user est pas dans le channel
		sendOneRPL(ERR_NOTONCHANNEL(user->getNickname(), cmdLine[1]), fd);
		return ;
	}
	if (cmdLine.size() == 2)
	{
		// envoyer rpl TOPIC si il y en a un, ou RPL_NOTOPIC si ya pas
		std::cout << server._channels[i]->getTopic() << std::endl;
		if (server._channels[i]->getTopic().empty() == true)
			sendOneRPL(RPL_NOTOPIC(user->getNickname(), cmdLine[1]), fd);
		else
			sendOneRPL(RPL_TOPIC(user->getNickname(), cmdLine[1], server._channels[i]->getTopic()), fd);
	}
	else if (cmdLine.size() == 3 && cmdLine[2].empty() == true)
	{
		if (server._channels[i]->getMode_t() == true && user->getisOperator() < 1)
			return ;
		// clear le topic
		std::cout << "remove topic" << std::endl;
		server._channels[i]->getTopic().clear();
		server._channels[i]->sendRPLtoChan(RPL_NOTOPIC(user->getNickname(), cmdLine[1]));
	}
	else
	{
		// set un nouveau topic ou remplacer l'ancien
		// ne pas oublier de prendre tout le topic et pas que le premier mot
		if (server._channels[i]->getMode_t() == true && user->getisOperator() < 1)
			return ;
		
		if (server._channels[i]->getTopic().empty() == false)
			server._channels[i]->getTopic().clear();

		std::string topic;
		for (size_t i = 2; i < cmdLine.size(); i++)
		{	
			topic += cmdLine[i];
			topic += " ";
		}
		
		server._channels[i]->setTopic(topic);
		server._channels[i]->sendRPLtoChan(RPL_TOPIC(user->getNickname(), cmdLine[1], server._channels[i]->getTopic()));
	}
	return ;
}