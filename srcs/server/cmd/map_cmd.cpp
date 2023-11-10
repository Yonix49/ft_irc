/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_cmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:07:09 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/10 12:37:05 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"



int Server::use_map_function(std::string buffer, int fd)
{
	(void)(fd);
	int flag = 0;
	std::vector<std::string> words = get_vector_ref(buffer);
	if (words.size() <= 0)
		return (1);
	for (std::map<std::string, CommandFunction>::const_iterator it = commandMap.begin(); it != commandMap.end(); ++it)
	{
		if (strncmp(it->first.c_str(), words[0].c_str(), strlen(words[0].c_str())) == 0)
		{
			CommandFunction commandFunction = it->second;
			commandFunction(buffer, fd);
			flag++;
			break;
		}
	}
	if (flag == 0)
	{
		User *user = Server::getUserNo(fd);
		sendOneRPL(ERR_UNKNOWNCOMMAND(user->getNickname(), words[0]), fd);
		return (1);
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
	commandMap["USER"] = &Server::HandleUserCommand;
	commandMap["userhost"] = &Server::HandleUserCommand;
	commandMap["JOIN"] = &Server::join;
	commandMap["INVITE"] = &Server::invite;
	commandMap["TOPIC"] = &Server::topic;
	commandMap["MODE"] = &Server::mode;
	commandMap["PART"] = &Server::part;
	commandMap["PRIVMSG"] = &Server::HandlePrivMessage;
	commandMap["NOTICE"] = &Server::HandleNoticeMessage;
	commandMap["PING"] = &Server::ping;
	commandMap["KICK"] = &Server::kick;
	commandMap["QUIT"] = &Server::quit;
	commandMap["PASS"] = &Server::HandlePassCommand;

}