/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irssi_check_connection.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:11:36 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/29 16:12:09 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int Server::irsii_argument_check(std::vector<std::string> words, int fd, User *user)
{
	if (!words.empty() && words.size() >= 11)
	{
		if (words[2] == "PASS")
		{
			std::cout << "je verif le mot de passe" << std::endl;
			if (words[3] == _password.c_str())
			{
				std::cout << "good mdp" << std::endl;
				user->incre_nc_check();
			}
			else
			{
				std::cout << "wrong mdp" << std::endl;
				return (1);
				/// ERR_PASSWDMISMATCH
			}
		}
		else
		{
			/// ERR_PASSWDMISMATCH
			return (1);
		}
		//
		if (words[4] == "NICK")
		{
			std::cout << "je suis ici check nick" << std::endl;
			if (check_nick(words[5], fd, user) != 0)
			{
				std::cerr << "Mauvais nickname" << std::endl;
				return (1);
				// ERR_NONICKNAMEGIVEN
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
			// ERR_NONICKNAMEGIVEN
			return (1);
		}
		std::cout << "good argument check irsii" << std::endl;
		return (0);
	}
	return (1);
	// Error Not enough arg
}
int Server::irssi_check(std::string str, int fd)
{
	std::vector<std::string> words = get_vector_ref(str);
	std::cout << "je suis ici irsii check connection " << std::endl;
	User *user = NULL; // Déclarer un pointeur vers un utilisateur
	user = getUserNo(fd);
	if (!user)
		return (1);
	if (irsii_argument_check(words, fd, user) != 0)
		return (1);

	user->set_in_server(true);
	return (0);
}
