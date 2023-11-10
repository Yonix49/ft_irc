/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_cmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:18:14 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/10 14:53:51 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"

void Server::HandleUserCommand(std::string param, int fd)
{
	Server &server = Server::getInstance();
	User *user = server.getUserNo(fd);
	std::vector<std::string> words = server.get_vector_ref(param);
	try
	{
		if (words.size() >= 1 && words[0] == "USER")
		{
			throw Error_rpl();
		}
		throw Error_rpl();
	}
	catch (const Error_rpl &ex)
	{
		sendOneRPL(ERR_ALREADYREGISTERED(user->getNickname()), fd);
	}
}
