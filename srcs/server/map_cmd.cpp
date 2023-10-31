/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_cmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:07:09 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/30 18:31:54 by mhajji-b         ###   ########.fr       */
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
		std::cout << "je suis iciiiii " << words[0] << " " << it->first << std::endl;
		std::cout << "strncmp ==" << strncmp(it->first.c_str(), words[0].c_str(), strlen(words[0].c_str())) << std::endl;

		if (strncmp(it->first.c_str(), words[0].c_str(), strlen(words[0].c_str())) == 0)
		{
			std::cout << "j'appel la fonction de la map associe " << std::endl;
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
// std::vector<std::string> cmdLine
void Server::initializeCommandMap()
{
	commandMap["NICK"] = &Server::HandleNickCommand;
	commandMap["user"] = &Server::HandleUserCommand;
	commandMap["userhost"] = &Server::HandleUserCommand;
	// commandMap["PASS"] = &Server::HandlePassCommand;

}

// Ajoutez d'autres commandes ici
void Server::HandleNickCommand(std::string param, int fd)
{
	// Server server;
	Server &server = Server::getInstance(); // Obtenez une référence à l'instance unique de la classe
	User *user = server.getUserNo(fd);
	std::vector<std::string> words = server.get_vector_ref(param);
	try
	{
		if (words.size() == 2 && words[0] == "NICK")
		{
			if (server.check_nick(words[1], fd, user) != 0)
				throw Error_rpl();
		}
	}
	catch (const Error_rpl &ex)
	{
		std::cerr << "Erreur : " << server.get_Error_user(fd) << std::endl;
	}
}
void Server::HandleUserCommand(std::string param, int fd)
{
	Server &server = Server::getInstance(); // Obtenez une référence à l'instance unique de la classe
	User *user = server.getUserNo(fd);
	std::vector<std::string> words = server.get_vector_ref(param);
	try
	{
		if (words.size() >= 1 && words[0] == "USER")
		{
			server.set_Error_user("ERR_ALREADYREGISTERED", fd);
			throw Error_rpl();
		}
		server.set_Error_user("ERR_ALREADYREGISTERED", fd);
		throw Error_rpl();
	}
	catch (const Error_rpl &ex)
	{
		std::cerr << "Erreur : " << server.get_Error_user(fd) << std::endl;
		sendOneRPL(ERR_ALREADYREGISTERED(user->getNickname()), fd);
	}
}
void Server::HandlePassCommand(std::string param, int fd)
{
	Server &server = Server::getInstance(); // Obtenez une référence à l'instance unique de la classe
	std::vector<std::string> words = server.get_vector_ref(param);
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
		std::cerr << "Erreur : " << server.get_Error_user(fd) << std::endl;
		sendOneRPL(server.get_Error_user(fd), fd);
	}
}