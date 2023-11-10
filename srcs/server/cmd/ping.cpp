/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:01:16 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/10 14:52:15 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"




void	Server::ping(std::string param, int fd)
{
	Server &server = Server::getInstance();
	User *user = server.getUserNo(fd);
	std::vector<std::string> cmdLine = server.get_vector_ref(param);
	
	if (user->get_check_in_server() == false)
	{
		sendOneRPL(ERR_NOTREGISTERED(user->getNickname(), "PING"), fd);
		return ;
	}
	if (cmdLine.size() < 2)
	{
		sendOneRPL(ERR_NOORIGIN(user->getNickname()), fd);
		return ;
	}
	if (cmdLine[1].compare("localhost"))
	{
		sendOneRPL(ERR_NOSUCHSERVER(user->getNickname()), fd);
		return ;
	}
	sendOneRPL(RPL_PONG, fd);
}	
