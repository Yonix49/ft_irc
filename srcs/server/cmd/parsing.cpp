/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:18:36 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/10 14:51:18 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"




int Server::check_password(std::vector<std::string> str)
{
	for (std::vector<std::string>::iterator it = str.begin(); it != str.end(); ++it)
	{
		if (it->find("PASS") != std::string::npos)
		{
			if (it + 1 != str.end())
			{
				if (_password.compare(*(it + 1)) == 0 || _password.empty() != 0)
					return (0);
				else
					return (1);
			}
		}
	}
	return (0);
}

int Server::check_nickname(std::vector<std::string> str)
{
	std::string nickname;

	for (std::vector<std::string>::iterator it = str.begin(); it != str.end(); ++it)
	{
		if (it->find("NICK") != std::string::npos)
		{
			if (it + 1 != str.end())
			{
				nickname = *(it + 1);
				break;
			}
		}
	}
	if (nickname.find(' ') != std::string::npos ||
		nickname.find(',') != std::string::npos ||
		nickname.find('*') != std::string::npos ||
		nickname.find('?') != std::string::npos ||
		nickname.find('@') != std::string::npos ||
		nickname.find('!') != std::string::npos ||
		nickname.find('.') != std::string::npos)
	{
		std::cout << "\033[1;31mcaracteres invalides trouves dans le nickname\033[0m" << std::endl;
		return (-2);
	}
	char first_char;
	first_char = nickname[0];
	if (first_char == '$' || first_char == ':' || first_char == '#')
	{
		std::cout << "\033[1;31mLe nickname start avec un char invalides\033[0m" << std::endl;
		return (-3);
	}
	_users.back().setNickname(nickname);
	return 0;
}