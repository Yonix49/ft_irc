/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 13:07:11 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/31 14:59:02 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include"server.hpp"

int	Server::topic(std::vector<std::string> cmdLine, int fd)
{
	User *user;
	
	user = getUserNo(fd); 

	if (cmdLine.size() == 1)
		return (-3); // need more params
	int i = channelExist(cmdLine[1]);
	if (i < 0)
		return (-1); // no such channel
	if (_channels[i]->isUserinchan(user->getNickname(), 0) == -1)
	{
		// check si le user est pas dans le channel
		sendOneRPL(ERR_NOTONCHANNEL(user->getNickname(), cmdLine[1]), fd);
		return (-2);
	}
	if (cmdLine.size() == 2)
	{
		// envoyer rpl TOPIC si il y en a un, ou RPL_NOTOPIC si ya pas
		std::cout << _channels[i]->getTopic() << std::endl;
		if (_channels[i]->getTopic().empty() == true)
			sendOneRPL(RPL_NOTOPIC(user->getNickname(), cmdLine[1]), fd);
		else
			sendOneRPL(RPL_TOPIC(user->getNickname(), cmdLine[1], _channels[i]->getTopic()), fd);
	}
	else if (cmdLine.size() == 3 && cmdLine[2].empty() == true && user->getisOperator() > 0)
	{
		// clear le topic
		_channels[i]->getTopic().clear();
		_channels[i]->sendRPLtoChan(RPL_NOTOPIC(user->getNickname(), cmdLine[1]));
	}
	else if (user->getisOperator() > 0)
	{
		// set un nouveau topic ou remplacer l'ancien
		if (_channels[i]->getTopic().empty() == false)
			_channels[i]->getTopic().clear();
		_channels[i]->setTopic(cmdLine[2]);
		_channels[i]->sendRPLtoChan(RPL_TOPIC(user->getNickname(), cmdLine[1], _channels[i]->getTopic()));
		// std::cout << _channels[i]->getTopic() << std::endl;
	}
	return (0);
}