/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nc_check_connection.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:14:40 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/07 10:53:47 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int Server::nc_check(std::string str, int fd)
{
	std::vector<std::string> words = get_vector_ref(str);
	User *user = getUserNo(fd);
	if (!user)
		return (1);
	// int flag = 0;

	if (user && user->get_nc_check() <= 2)
	{
		try
		{
			if (!words.empty() && words.size() > 0)
			{
				if (words[0] == "PASS" && user->get_nc_check() == 0)
				{
					std::cout << "je suis mdp " << std::endl;
					if (words.size() == 2 && words[1] == _password.c_str())
					{
						std::cout << "good password" << std::endl;
						user->incre_nc_check();
					}
					else
					{
						sendOneRPL(ERR_PASSWDMISMATCH(user->getNickname()), fd);
						throw Error_rpl();
					}
				}
				else if (words[0] == "NICK" && user->get_nc_check() == 1)
				{
					if (words.size() == 2)
					{
						if (check_nick(words[1], fd, user) != 0)
						{
							throw Error_rpl();
							
						}
						else
						{
							user->incre_nc_check();
							user->setNickname(words[1]);
							// std::cout << "good NICK " << words[1] << std::endl;
						}
					}
					else
						sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "NICK"), fd);
				}
				else if (words[0] == "USER" && user->get_nc_check() == 2)
				{ 
					if (check_user_nc(fd, user, words) != 0)
					{
						std::cerr << "Erreur commande USER" << std::endl;
						throw Error_rpl();
					}
					else
					{
						user->incre_nc_check();
					}
				}
				else
				{
					sendOneRPL(ERR_UNKNOWNCOMMAND(user->getNickname(), words[0]), fd);
					throw Error_rpl();
				}
			}
		}
		catch (const Error_rpl &ex)
		{
			std::cerr << "Erreur : " << get_Error_user(fd) << std::endl;
		}
	}

	if (user->get_nc_check() == 3)
		user->set_in_server(true);
	return 0;
}
int Server::check_user_nc(int fd, User *user, std::vector<std::string> words)
{

	if (words.size() == 5)
	{
		// std::cout << "je suis la" << std::endl;
		if (words[1].empty() || words[1].length() <= 1)
		{
			std::cout << "je suis la 2" << words[1] << std::endl;
			sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "USER"), fd);
			set_Error_user("ERR_NEEDMOREPARAMS", fd);
			return (1);
		}
		else  // Username // USERLEN=10
		{
			if (words[1].length() > 10)
				words[1] = words[1].substr(0, 10);
			std::cout << "set username" << words[1] << std::endl;
			user->setUsername(words[1]);
		}
		if (words[2] != "0" && words[3] != "*")
		{
			sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "USER"), fd);
			set_Error_user("ERR_NEEDMOREPARAMS", fd);
			return (1);
		}
		else if (!words[4].empty())
		{
			if (words[4].find(' ') != std::string::npos)
			{
				words[4] = ":" + words[4];
			}
			user->setRealname(words[4]);
			std::cout << "real name " << words[4] << std::endl;
			return (0);		
		}
	}
	else
	{
		std::cout << "Usage of USER Parameters: <username> 0 * <realname>" << std::endl;
		set_Error_user("ERR_NEEDMOREPARAMS", fd);
		sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "USER"), fd);
		return (1);
	}
	return (0);
}