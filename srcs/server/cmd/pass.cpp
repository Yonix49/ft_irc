/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:17:42 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/10 12:37:27 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"




void Server::HandlePassCommand(std::string param, int fd)
{
	Server &server = Server::getInstance(); // Obtenez une référence à l'instance unique de la classe
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