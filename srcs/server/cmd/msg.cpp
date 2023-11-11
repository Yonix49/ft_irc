/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 18:09:38 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/11 15:07:42 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"

void Server::HandlePrivMessage(std::string param, int fd)
{
	User	*user;

	Server &server = Server::getInstance();
	user = server.getUserNo(fd);
	std::vector<std::string> words = server.get_vector_ref(param);
	try
	{
		if (words.size() >= 3 && words[0] == "PRIVMSG")
		{	
			if (server.message_user(words, fd, user, param) != 0)
				throw Error_rpl();
		}
		else
		{
			sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "PRIVMSG"), fd);
			throw Error_rpl();
		}
	}
	catch (const Error_rpl &ex) {
	}
}
void Server::HandleNoticeMessage(std::string param, int fd)
{
	User	*user;

	Server &server = Server::getInstance();
	user = server.getUserNo(fd);
	std::vector<std::string> words = server.get_vector_ref(param);
	try
	{
		if (words.size() >= 3 && words[0] == "NOTICE")
		{
			if (server.message_notice(words, fd, user, param) != 0)
				throw Error_rpl();
		}
		else
		{
			sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "NOTICE"), fd);
			throw Error_rpl();
		}
	}
	catch (const Error_rpl &ex) {
	}
}
int Server::message_notice(std::vector<std::string> words, int fd, User *user,
		std::string param)
{
	int	flag;
	int	gotten_fd;

	(void)(fd);
	std::string to_compare;
	std::string my_nick = user->getNickname();
	flag = 0;
	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		User &currentUser = *it;
		gotten_fd = currentUser.getFd();
		to_compare = currentUser.getNickname();
		if (to_compare == words[1])
		{
			flag++;
			break ;
		}
	}
	if (flag == 0)
	{
		sendOneRPL(ERR_WASNOSUCHNICK(user->getNickname(), words[1]), fd);
		return (1);
	}
	size_t startPos = param.find(":");
	std::string resultat;
	if (startPos != std::string::npos)
	    resultat = param.substr(startPos + 1);
	else
	{
	    startPos = param.find(to_compare);

	    if (startPos != std::string::npos)
	        resultat = param.substr(startPos + to_compare.length() + 1);
	}
	if (flag == 1)
		sendOneRPL(PRIVMSG(user->getNickname(), user->getUsername(), to_compare,
					resultat), gotten_fd);
	return (0);
}
int Server::message_user(std::vector<std::string> words, int fd, User *user,
		std::string param)
{	
	std::string	resultat;
	size_t		startPos;
	std::string to_compare;	
	std::string my_nick = user->getNickname();
	std::string messageBot;
	int flag = 0;
	int channel = 0;
	int gotten_fd;

	if (words.size() > 2 && words[1].compare("$BOT") == 0)
	{
		std::string messageBot("Salut je suis Johnny le bot !!");
		std::string nameBot("$BOT");
		sendOneRPL(PRIVMSG(user->getNickname(), user->getUsername(), nameBot,
					messageBot), fd);
		return (0);
	}
	else
	{
		for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
		{
			User &currentUser = *it;
			gotten_fd = currentUser.getFd();
			to_compare = currentUser.getNickname();
			if (to_compare == words[1])
			{
				flag++;
				break ;
			}
		}
	}

	channel = Server::channelExist(words[1]);
	if (words[1][0] != '#' && flag == 0)
	{
		sendOneRPL(ERR_WASNOSUCHNICK(user->getNickname(), words[1]), fd);
		return (1);
	}
	if (words[1][0] == '#' && channel == -1)
	{
		sendOneRPL(ERR_NOSUCHCHANNEL(user->getNickname(), words[1]), fd);
		return (1);
	}
	startPos = param.find(":");
	
	if (channel != -1) // Cas channel
	{
	    startPos = param.find(_channels[channel]->getName());
	
	    if (startPos != std::string::npos)
	    {
	        resultat = param.substr(startPos + _channels[channel]->getName().length() + 1);
	    }
	}
	else if (startPos != std::string::npos) // Cas irssi
	{
	    resultat = param.substr(startPos + 1);
	}
	else // Cas NC
	{
	    startPos = param.find(to_compare);
	
	    if (startPos != std::string::npos)
	        resultat = param.substr(startPos + to_compare.length() + 1);
	}
	if (flag == 1)
	{
		sendOneRPL(PRIVMSG(user->getNickname(), user->getUsername(), to_compare,
					resultat), gotten_fd);
	}
	else if (channel != -1
			&& _channels[channel]->isUserinchan(user->getNickname(), 0) != -1)
	{
			_channels[channel]->sendMSGtoChan(PRIVMSG(user->getNickname(),
				user->getUsername(), _channels[channel]->getName(),
				resultat), fd);
	}
	else
	{
		sendOneRPL(ERR_NOTONCHANNEL(user->getUsername(),
					_channels[channel]->getName()), fd);
	}
	return (0);
}
