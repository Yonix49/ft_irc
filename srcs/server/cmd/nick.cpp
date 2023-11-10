/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:16:25 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/10 15:32:06 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"

void Server::HandleNickCommand(std::string param, int fd)
{
	Server &server = Server::getInstance();
	User *user = server.getUserNo(fd);
	std::vector<std::string> words = server.get_vector_ref(param);
	try
	{
		if (words.size() == 2 && words[0] == "NICK")
		{
			if (server.check_nick(words[1], fd, user) != 0)
				throw Error_rpl();
		}
		else
		{
			sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "NICK"), fd);
			throw Error_rpl();

		}
	}
	catch (const Error_rpl &ex) {
	}
}

int Server::check_nick(std::string nickname, int fd, User *user)
{
	std::string to_compare;

	int gotten_fd;
	if (nickname.find(' ') != std::string::npos ||
		nickname.find(',') != std::string::npos ||
		nickname.find('*') != std::string::npos ||
		nickname.find('?') != std::string::npos ||
		nickname.find('@') != std::string::npos ||
		nickname.find('!') != std::string::npos)
	{
		std::cerr << "Invalid character in nickname" << std::endl;
		sendOneRPL(ERR_ERRONEUSNICKNAME(user->getNickname()), fd);
		set_Error_user("ERR_ERRONEUSNICKNAME", fd);
		return 1;
	}

	char c = nickname[0];
	if (c == ':' || c == '#' || c == '&')
	{
		std::cerr << "Invalid character in nickname" << std::endl;
		sendOneRPL(ERR_ERRONEUSNICKNAME(user->getNickname()), fd);
		set_Error_user("ERR_ERRONEUSNICKNAME", fd);
		return (1);
	}
	if (nickname.compare("$BOT") == 0)
	{
		std::cerr << "Invalid character in nickname" << std::endl;
		sendOneRPL(ERR_ERRONEUSNICKNAME(user->getNickname()), fd);
		set_Error_user("ERR_ERRONEUSNICKNAME", fd);
		return (1);
	}
	if (nickname.length() > 20)
	{
		std::cerr << "Nickname is too long" << std::endl;
		sendOneRPL(ERR_ERRONEUSNICKNAME(user->getNickname()), fd);
		set_Error_user("ERR_ERRONEUSNICKNAME", fd);
		return (1);
	}
	for (int i = 0; nickname[i]; i++)
	{
		if (nickname[i] < 32 || nickname[i] > 126)
		{
			std::cerr << "Invalid character in nickname" << std::endl;
			sendOneRPL(ERR_ERRONEUSNICKNAME(user->getNickname()), fd);
			set_Error_user("ERR_ERRONEUSNICKNAME", fd);
			return (1);
		}
	}
	int flag = 0;
	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		User &currentUser = *it;
		to_compare = currentUser.getNickname();
		gotten_fd = currentUser.getFd();
		if (to_compare == nickname && gotten_fd != fd && is_connected(fd) == false)
		{
			sendOneRPL(ERR_NICKNAMEINUSE(to_compare), fd); 	
			nickname += "_";
			user->setNickname(nickname);
			it = _users.begin();
			flag = 1;
		}
	}
	if (flag == 1)
	{	
		sendOneRPL(NICK(user->getNickname(), user->getUsername(), nickname), fd); 	
		return (0);
	}
	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		User &currentUser = *it;
		to_compare = currentUser.getNickname();
		gotten_fd = currentUser.getFd();
		if (to_compare == nickname && gotten_fd != fd)
		{
			sendOneRPL(ERR_NICKNAMEINUSE(to_compare), fd); 	
			return (1);
		}
	}
	sendOneRPL(NICK(user->getNickname(), user->getUsername(), nickname), fd); 	
	user->setNickname(nickname);
	return (0);
}