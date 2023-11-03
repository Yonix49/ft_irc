/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 18:09:38 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/03 19:34:38 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "server.hpp"

void Server::HandlePrivMessage(std::string param, int fd)
{
	Server &server = Server::getInstance(); // Obtenez une référence à l'instance unique de la classe
	User *user = server.getUserNo(fd);
	std::vector<std::string> words = server.get_vector_ref(param);
	try
	{
		if (words.size() >= 3 && words[0] == "PRIVMSG")
		{
			std::cout << "je suis dans privmsg" << "size == " << words.size() << std::endl;
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
		std::cerr << "Erreur : " << server.get_Error_user(fd) << std::endl;
	}
}
void Server::HandleNoticeMessage(std::string param, int fd)
{
	Server &server = Server::getInstance(); // Obtenez une référence à l'instance unique de la classe
	User *user = server.getUserNo(fd);
	std::vector<std::string> words = server.get_vector_ref(param);
	try
	{
		if (words.size() >= 3 && words[0] == "NOTICE")
		{
			std::cout << "je suis dans NOTICE" << "size == " << words.size() << std::endl;
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
		std::cerr << "Erreur : " << server.get_Error_user(fd) << std::endl;
	}
}
int Server::message_notice(std::vector<std::string> words, int fd, User *user, std::string param)
{	
	(void)(fd);
	std::string to_compare;
	std::string my_nick = user->getNickname();
	int flag = 0;
	int gotten_fd;
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
	size_t startPos = param.find(":"); // Recherche de l'indice de début
	std::string resultat;
	if (startPos != std::string::npos) 
	{
     	resultat = param.substr(startPos +1); // +2 pour ignorer ": "
    	std::cout << "Message : " << resultat << std::endl;
	}
	if (flag == 1)
	{
		std::string message = "Private message from " + user->getNickname();
		sendOneRPL(PRIVMSG(user->getNickname(), user->getUsername(), to_compare, resultat), gotten_fd);
	}
	return (0);
}
int Server::message_user(std::vector<std::string> words, int fd, User *user, std::string param)
{	
	(void)(fd);
	std::string to_compare;
	std::string my_nick = user->getNickname();
	int flag = 0;
	int channel = 0;
	int gotten_fd;
	channel = Server::channelExist(words[1]);
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
	if (words[1].c_str()[0] == '#' && channel == -1 && flag == 0)
	{
		std::cout << "je suis ici" << std::endl;
		sendOneRPL(ERR_NOSUCHCHANNEL(user->getNickname(), words[1]), fd);
		return (1);
	}
	else if (flag == 0 && channel == -1)
	{
		sendOneRPL(ERR_WASNOSUCHNICK(user->getNickname(), words[1]), fd);
		return (1);
	}
	
	
	size_t startPos = param.find(":"); // Recherche de l'indice de début
	std::string resultat;
	if (startPos != std::string::npos) 
	{
     	resultat = param.substr(startPos +1); // +2 pour ignorer ": "
    	std::cout << "Message : " << resultat << std::endl;
	}
	
	if (flag == 1)
	{
		std::string message = "Private message from " + user->getNickname();
		sendOneRPL(PRIVMSG(user->getNickname(), user->getUsername(), to_compare, resultat), gotten_fd);
	}
	else if (channel != -1 && _channels[channel]->isUserinchan(user->getNickname(), 0) != -1)
	{
		std::string message = "Channel message from " + user->getNickname();
		_channels[channel]->sendMSGtoChan(PRIVMSG(user->getNickname(), user->getUsername(), _channels[channel]->getName(), resultat), fd);
	}
	else
	{
		sendOneRPL(ERR_NOTONCHANNEL(user->getUsername()
		, _channels[channel]->getName()), fd);
		//A changer
		std::cout << "No present in the channel" << std::endl;
	}
	return (0);
}