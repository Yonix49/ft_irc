/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nc_check_connection.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:14:40 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/29 16:15:20 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"


int Server::nc_check(std::string str, int fd)
{
	std::vector<std::string> words = get_vector_ref(str);
	std::cout << "je suis ici nc_check " << std::endl;
	User *user = NULL; // Déclarer un pointeur vers un utilisateur
	user = getUserNo(fd);
	if (!user)
		return (1);

	if (user && user->get_nc_check() <= 2)
	{
		if (!words.empty() && words.size() > 0)
		{
			std::cout << "password " << _password << std::endl;
			if (words[0] == "PASS" && user->get_nc_check() == 0)
			{
				if (words.size() == 2 && words[1] == _password.c_str())
				{
					std::cout << "good password" << std::endl;
					user->incre_nc_check();
				}
				else
				{
					// ERR_PASSWDMISMATCH
				}
			}
			else if (words[0] == "NICK" && user->get_nc_check() == 1)
			{
				std::cout << "je suis entrer dans NICK" << std::endl;
				if (words.size() == 2)
				{
					if (check_nick(words[1], fd, user) != 0)
					{
						std::cerr << "Mauvais nickname" << std::endl;
						// ERR_NONICKNAMEGIVEN
					}
					else
					{
						user->incre_nc_check();
						user->setNickname(words[1]);
						std::cout << "good NICK " << words[1] << std::endl;
					}
					// }
					// develop la logique ici
				}
			}
			else if (words[0] == "USER" && user->get_nc_check() == 2)
			{ // develop la logique ici

				std::cout << "je suis entrer dans USER" << std::endl;
				if (check_user_nc(fd, user, words) != 0)
				{
					std::cerr << "Erreur commande USER" << std::endl;
				}
				else
				{
					user->incre_nc_check();
				}
				// Je dois check le bon USER ici
			}
		}
	}
	std::cout << "nc == " << user->get_nc_check() << std::endl;
	if (user->get_nc_check() == 3)
		user->set_in_server(true);
	return 0;
}
int Server::check_user_nc(int fd, User *user, std::vector<std::string> words)
{

	if (words.size() == 5)
	{
		std::cout << "je suis la" << std::endl;
		// std::cout << "le username rentrer avant op == " << words[1] << std::endl;
		if (words[1].empty() || words[1].length() <= 1)
		{
			std::cout << "je suis la 2" << words[1] << std::endl;
			return (1);
			// Le serveur rejette avec rep ERR_NEEDMOREPARAMS
		}
		if (words[1].length() > 10) // Username // USERLEN=10
		{
			words[1] = words[1].substr(0, 10);
			user->setUsername(words[1]);
			std::cout << words[1] << " length == " << words[1].length() << std::endl;
		}
		else if (words[2] != "0" && words[3] != "*")
		{
			std::cout << "je suis la 3" << std::endl;

			std::cout << "mauvais parametres" << std::endl;
			return (1);
		}
		else if (!words[4].empty())
		{
			if (words[4].find(' ') != std::string::npos)
			{
				words[4] = ":" + words[4];
			}
			// "kgezgin 0 * :kenan GEZGIN"
			user->setRealname(words[4]);
			std::cout << "real name " << words[4] << std::endl;
		}
	}
	else
	{
		std::cout << "je suis la 4" << std::endl;
		std::cout << "Usage of USER Parameters: <username> 0 * <realname>" << std::endl;
		// ERR_NEEDMOREPARAMS
		return (1);
	}
	// si le gars a deja utiliser USER au debut lors de nc msg derror si il ressaie de l'utiliser ERR_ALREADYREGISTERED
	(void)(user);
	(void)(fd);
	return (0);
}