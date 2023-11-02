/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 18:09:38 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/02 18:26:24 by kgezgin          ###   ########.fr       */
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

int Server::message_user(std::vector<std::string> words, int fd, User *user, std::string param)
{	
	(void)(fd);
	std::string to_compare;
	std::string my_nick = user->getNickname();
	int flag = 0;
	int channel = 0;
	int gotten_fd;
	// std::vector<Channel *>				_channels;
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
	if (flag == 0 && channel == -1)
	{
		//ERR_NOSUCHNICK
		//ERRNOCHANN
		std::cout << "l'utilisateur existe pas et chan existe pas " << std::endl;
	}
	std::string resultat;
	size_t pos = param.find(words[1]);
	if (pos != std::string::npos) 
	{
	    resultat = param.substr(pos + 7); // "+ 7" pour ignorer "PRIVMSG" et l'espace qui suit
	    std::cout << "Résultat : " << resultat << std::endl;
	}
	if (flag == 1)
	{
		std::string message = "Private message from " + user->getNickname();
		sendOneRPL(PRIVMSG(user->getNickname(), user->getUsername(), to_compare, resultat), gotten_fd);
	}
	else if (channel != -1)
	{
		std::string message = "Channel message from " + user->getNickname();
		_channels[channel]->sendMSGtoChan(PRIVMSG(user->getNickname(), user->getUsername(), _channels[channel]->getName(), resultat), fd);
	}	
	return (0);
}