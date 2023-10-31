/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irssi_check_connection.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:11:36 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/31 22:22:12 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int Server::irsii_argument_check(std::vector<std::string> words, int fd, User *user)
{
	
	if (!words.empty())
	{
		if (words[2] == "PASS")
		{
			
			if (words[3] == _password.c_str())
			{
				user->incre_nc_check();
			}
			else
			{
				std::cout <<  "111 mdp == " << words[3] << "|| nickname ==" << user->getNickname() << "  APRES =========================================" << std::endl;
				set_Error_user("ERR_PASSWDMISMATCH", fd);
				sendOneRPL(ERR_PASSWDMISMATCH(user->getNickname()), fd);
				return (1);
			}
		}
		else
		{
			std::cout <<  "222 mdp == " << words[3] << "|| nickname ==" << user->getNickname() << "  APRES =========================================" << std::endl;
			set_Error_user("ERR_PASSWDMISMATCH", fd);
			
			sendOneRPL(ERR_PASSWDMISMATCH(user->getNickname()), fd);
			return (1);
		}
		//
		if (words[4] == "NICK")
		{
			if (check_nick(words[5], fd, user) != 0)
			{
				set_Error_user("ERR_NONICKNAMEGIVEN", fd);
				return (1);
			}
			else
			{
				user->incre_nc_check();
				user->setNickname(words[5]);
				std::cout << "good NICK " << words[5] << std::endl;
			}
		}
		else
		{
			sendOneRPL(ERR_NONICKNAMEGIVEN(user->getNickname()), fd);
			set_Error_user("ERR_NONICKNAMEGIVEN", fd);
			return (1);
		}
	}
	return (0);
}

int Server::irssi_check(std::string str, int fd)
{
	std::vector<std::string> words = get_vector_ref(str);
	User *user = NULL; // Déclarer un pointeur vers un utilisateur
	user = getUserNo(fd);
	try
	{
		if (irsii_argument_check(words, fd, user) != 0)
		{
			throw Error_rpl();
		}
	}
	catch (const Error_rpl &ex)
	{
		std::cerr << "Erreur : " << get_Error_user(fd) << std::endl;
		return (1);
	}

	// std::cout << user->get_check_in_server() << "AVANT =========================================" << std::endl;
	user->set_in_server(true);
	// std::cout << user->get_check_in_server() << "APRES =========================================" << std::endl;
	return (0);
}
