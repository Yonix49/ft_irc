/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 10:01:16 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/10 12:37:29 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"




void	Server::ping(std::string param, int fd)
{
	std::cout << "OOOOOOOOOOOOOOOOOOOOOOOo entree dans la fonction" << std::endl;
	Server &server = Server::getInstance();
	User *user = server.getUserNo(fd);
	// on peux protger
	std::vector<std::string> cmdLine = server.get_vector_ref(param);
	
	if (user->get_check_in_server() == false)
	{
		sendOneRPL(ERR_NOTREGISTERED(user->getNickname(), "PING"), fd);
		std::cout << "OOOOOOOOOOOOOOOOOOOOOOOo return 3" << std::endl;
		return ;
	}
	if (cmdLine.size() < 2)
	{
		sendOneRPL(ERR_NOORIGIN(user->getNickname()), fd);
		std::cout << "OOOOOOOOOOOOOOOOOOOOOOOo return 2" << std::endl;
		return ;
	}
	if (cmdLine[1].compare("localhost"))
	{
		sendOneRPL(ERR_NOSUCHSERVER(user->getNickname()), fd);
		std::cout << "OOOOOOOOOOOOOOOOOOOOOOOo return 4" << std::endl;
		return ;
	}
	sendOneRPL(RPL_PONG, fd);
	std::cout << "OOOOOOOOOOOOOOOOOOOOOOOo NO return"<< std::endl;
}	
