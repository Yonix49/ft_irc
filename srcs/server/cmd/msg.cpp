/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 18:09:38 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/10 12:37:10 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"



#include <ctime>
void Server::HandlePrivMessage(std::string param, int fd)
{
	User	*user;

	Server &server = Server::getInstance();
		// Obtenez une référence à l'instance unique de la classe
	user = server.getUserNo(fd);
	std::vector<std::string> words = server.get_vector_ref(param);
	try
	{
		if (words.size() >= 3 && words[0] == "PRIVMSG")
		{
			// std::cout << "je suis dans privmsg"
						// << "size == " << words.size() << std::endl;
			if (server.message_user(words, fd, user, param) != 0)
				throw Error_rpl();
		}
		else
		{
			sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "PRIVMSG"), fd);
			throw Error_rpl();
		}
	}
	catch (const Error_rpl &ex)
	{
	}
}
void Server::HandleNoticeMessage(std::string param, int fd)
{
	User	*user;

	Server &server = Server::getInstance();
		// Obtenez une référence à l'instance unique de la classe
	user = server.getUserNo(fd);
	std::vector<std::string> words = server.get_vector_ref(param);
	try
	{
		if (words.size() >= 3 && words[0] == "NOTICE")
		{
			std::cout << "je suis dans NOTICE"
						<< "size == " << words.size() << std::endl;
			if (server.message_notice(words, fd, user, param) != 0)
				throw Error_rpl();
		}
		else
		{
			sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "NOTICE"), fd);
			throw Error_rpl();
		}
	}
	catch (const Error_rpl &ex)
	{
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
			std::cout << words[1] << "words === " << std::endl;
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
	{
	    resultat = param.substr(startPos + 1);
	}
	else
	{
	    startPos = param.find(to_compare); // Recherche de l'indice de début

	    if (startPos != std::string::npos)
	    {
	        resultat = param.substr(startPos + to_compare.length() + 1);
	    }
	}
	if (flag == 1)
	{
		std::string message = "Private message from " + user->getNickname();
		sendOneRPL(PRIVMSG(user->getNickname(), user->getUsername(), to_compare,
					resultat), gotten_fd);
	}
	return (0);
}
int Server::message_user(std::vector<std::string> words, int fd, User *user,
		std::string param)
{
	(void)(fd);
	std::string to_compare;	
	std::string my_nick = user->getNickname();
	std::string messageBot;

	int flag = 0;
	int channel = 0;
	// int bot = 0;

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
	
	// std::cout << "je suis laaaaaaaa" << param << "BUFFER  dans msg ========== " << std::endl;
	std::string resultat; // Déclarez resultat ici, en dehors des blocs if
	size_t startPos;
	startPos = param.find(":"); // Recherche de l'indice de début
	
	if (channel != -1) // Cas channel
	{
	    // Réinitialisez startPos
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
	    // Réinitialisez startPos
	    startPos = param.find(to_compare);
	
	    if (startPos != std::string::npos)
	    {
	        resultat = param.substr(startPos + to_compare.length() + 1);
	    }
	}
	std::cout << "flag == " << flag << std::endl;
	if (flag == 1)
	{
		std::cout << "Je suis ici khey " << std::endl;
		std::cout << "NICKname " << user->getNickname() << std::endl;
		std::cout << "getUsername " <<  user->getUsername() << std::endl;
		std::cout << "destinateure " << to_compare << std::endl;
		std::cout << "message " << resultat << std::endl;

		// if (bot == 1)
		// 	resultat = "Salut, je suis Johnny le bot !!";
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
		std::cout << "No present in the channel" << std::endl;
	}
	return (0);
}