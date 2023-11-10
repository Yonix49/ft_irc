/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:17:42 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/10 14:52:01 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"

void Server::HandlePassCommand(std::string param, int fd)
{
	Server &server = Server::getInstance();
	std::vector<std::string> words = server.get_vector_ref(param);
	User *user = server.getUserNo(fd);
    try
	{
		if (words.size() >= 1 && words[0] == "PASS")
		{
			server.set_Error_user("ERR_ALREADYREGISTERED", fd);
			throw Error_rpl();
		}
		server.set_Error_user("ERR_ALREADYREGISTERED", fd);
		throw Error_rpl();
	}
	catch (const Error_rpl &ex)
	{
     	sendOneRPL(ERR_ALREADYREGISTERED(user->getNickname()), fd);
	}
}
