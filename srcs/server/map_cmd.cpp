/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_cmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:07:09 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/29 17:10:19 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
int Server::use_map_function(std::string buffer, int fd)
{
	(void)(fd);
	std::vector<std::string> words = get_vector_ref(buffer);
	if (words.size() <= 0)
		return (1);
	for (std::map<std::string, CommandFunction>::const_iterator it = commandMap.begin(); it != commandMap.end(); ++it)
	{
		// std::cout << "it->first " << it->first << "buffer " << words[0].c_str() << std::endl;

		std::cout << "strcmp " << strncmp(it->first.c_str(), words[0].c_str(), strlen(words[0].c_str())) << std::endl;
		if (strncmp(it->first.c_str(), words[0].c_str(), strlen(words[0].c_str())) == 0) // A changer cest bizzare
		{
			std::cout << "je suis dans la condition strncmp" << std::endl;
			// Vous avez trouvé la commande correspondante
			CommandFunction commandFunction = it->second;
			commandFunction(buffer, fd);
			break;
		}
	}
	return (0);
}

void Server::addCommand(const std::string &command, CommandFunction function)
{
	commandMap[command] = function;
}

void Server::initializeCommandMap()
{
	commandMap["NICK"] = &Server::HandleNickCommand;
}

// Ajoutez d'autres commandes ici
void Server::HandleNickCommand(std::string param, int fd)
{
	// Server server;
	std::cout << "param == " << param << std::endl;
	Server &server = Server::getInstance(); // Obtenez une référence à l'instance unique de la classe
	User *user = server.getUserNo(fd);
	std::vector<std::string> words = server.get_vector_ref(param);
	std::cout << "dans hd nickkkkkkkkkkkkkkkkk"
			  << "size" << words.size() << std::endl;
	if (words.size() == 2 && words[0] == "NICK")
	{
		if (server.check_nick(words[1], fd, user) != 0)
		{
			std::cerr << "Mauvais nickname" << std::endl;
			// ERR_NONICKNAMEGIVEN
		}
		else
		{
			std::cout << "set this username" << words[1] << std::endl;
			user->setNickname(words[1]);
		}
	}
}
