/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:13:50 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/02 19:02:38 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void	Server::kick(std::string param, int fd)
{
	Server &server = Server::getInstance();
	User *user = server.getUserNo(fd);
	std::vector<std::string> cmdLine = server.get_vector_ref(param);

	if (cmdLine.size() < 2)
	{
		sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "PART"), fd);
		return ;
	}
	
	std::string reason("");
	if (cmdLine.size() > 2)
		reason = cmdLine[2];
	int i = server.channelExist(cmdLine[1]);
	if (i == -1)
	{
		sendOneRPL(ERR_NOSUCHCHANNEL(user->getNickname(), cmdLine[1]), fd);
		return ;
	}
	// le user doit forcement etre un operator
	// mais, un operator ne peux pas kick un foundateur
	// on ne peux jamais reessendre a 0 users en utilisant kick donc pas besoin de verif


}
