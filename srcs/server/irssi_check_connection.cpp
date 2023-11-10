/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irssi_check_connection.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:11:36 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/10 14:54:37 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "server.hpp"

int Server::irssi_check(std::string str, int fd)
{
	std::vector<std::string> words = get_vector_ref(str);
	User *user = NULL;
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
		return (1);
	}
	if (user->get_nc_check() == 2)
		user->set_in_server(true);
	return (0);
}
int Server::irsii_argument_check(std::vector<std::string> words, int fd, User *user)
{
	int flag = 0;
	std::vector<std::string>::iterator it;
	for (it = words.begin(); it != words.end(); ++it) 
	{
		if (it->compare("CAP") == 0)
		{
			flag++;	
		}
    }
	if (flag == 1)
	{	
		if (Cap_case(words, fd, user) != 0)
		{
			return (1);
		}
	}
	else if (flag == 0) // No cap case 
	{
		if (No_Cap_case(words, fd, user) != 0)
		{
			return (1);
		}
	}
	return (0);
}
int Server::Cap_case(std::vector<std::string> words, int fd, User *user)
{
	if (!words.empty() && words.size() >= 6)
	{
		if (words[2] == "PASS")
		{
			if (words[3] == _password.c_str())
			{
				user->incre_nc_check();
			}
			else
			{
				sendOneRPL(ERR_PASSWDMISMATCH(user->getNickname()), fd);
				return (1);
			}
		}
		else
		{
			sendOneRPL(ERR_PASSWDMISMATCH(user->getNickname()), fd);
			return (1);
		}
		if (words[4] == "NICK")
		{
			if (check_nick(words[5], fd, user) != 0)
				return (1);
			user->incre_nc_check();
		}
		else
		{
			sendOneRPL(ERR_NONICKNAMEGIVEN(user->getNickname()), fd);
			return (1);
		}
		user->setUsername(words[6]);
		user->setRealname(words[6]);
	}
	return (0);
}
int Server::No_Cap_case(std::vector<std::string> words, int fd, User *user)
{
	if (!words.empty() && words.size() >= 6)
	{
		if (words[0] == "PASS")
		{
			if (words[1] == _password.c_str())
			{
				user->incre_nc_check();
			}
			else
			{
				sendOneRPL(ERR_PASSWDMISMATCH(user->getNickname()), fd);
				return (1);
			}
		}
		else
		{
			sendOneRPL(ERR_PASSWDMISMATCH(user->getNickname()), fd);
			return (1);
		}
		if (words[2] == "NICK")
		{
			if (check_nick(words[3], fd, user) != 0)
			{
				return (1);
			}
			user->incre_nc_check();
		}
		else
		{
			sendOneRPL(ERR_NONICKNAMEGIVEN(user->getNickname()), fd);
			return (1);
		}
		user->setUsername(words[5]);
		user->setRealname(words[5]);
	}
	return (0);
}
