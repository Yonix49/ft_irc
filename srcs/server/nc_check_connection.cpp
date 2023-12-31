/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nc_check_connection.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:14:40 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/11 15:07:24 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int Server::nc_check(std::string str, int fd)
{
	std::vector<std::string> words = get_vector_ref(str);
	User *user = getUserNo(fd);
	if (!user)
		return (1);
	// if (flag == 0)
	std::map<int, MyData>::iterator it = _buffer_stock.find(fd);
	if (it->second.flag == 0)
	{
		if (nc_not_ctrl_d(words, fd, user) != 0)
			return (1);
	}
	else if (it->second.flag == 1)
	{
		if (nc_ctrl_d_case(words, fd, user) != 0)
		{
			return (1);
		}
	}
	if (user->get_nc_check() == 3)
		user->set_in_server(true);
	return 0;
}
int Server::nc_not_ctrl_d(std::vector<std::string> words, int fd , User *user)
{
	if (user && user->get_nc_check() <= 2)
	{
		try
		{
			if (!words.empty() && words.size() > 0)
			{
				if (words[0] == "PASS" && user->get_nc_check() == 0)
				{
					if (words.size() == 2 && words[1].compare(_password) == 0)
					{
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
					if (words.size() >= 2)
					{
						if (check_nick(words[1], fd, user) != 0)
						{
							throw Error_rpl();							
						}
						else
						{
							user->incre_nc_check();
							user->setNickname(words[1]);
						}
					}
					else
						sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "NICK"), fd);
				}
				else if (words[0] == "USER" && user->get_nc_check() == 2)
				{ 
					if (check_user_nc(fd, user, words) != 0)
					{
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
		}
	}
	return (0);
}



int Server::check_user_nc(int fd, User *user, std::vector<std::string> words)
{

	if (words.size() == 5)
	{
		if (words[1].empty() || words[1].length() <= 1)
		{
			sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "USER"), fd);
			return (1);
		}
		else 
		{
			if (words[1].length() > 10)
				words[1] = words[1].substr(0, 10);
			user->setUsername(words[1]);
		}
		if (words[2] != "0" && words[3] != "*")
		{
			sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "USER"), fd);
			return (1);
		}
		else if (!words[4].empty())
		{
			if (words[4].find(' ') != std::string::npos)
			{
				words[4] = ":" + words[4];
			}
			user->setRealname(words[4]);
			return (0);		
		}
	}
	else
	{
		sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "USER"), fd);
		return (1);
	}
	return (0);
}

int Server::nc_ctrl_d_case(std::vector<std::string> words, int fd, User *user)
{
	std::vector<std::string>::iterator it;
	std::vector<std::string> elementsApresUser;
	int							i = 0;
	for (it = words.begin(); it != words.end(); ++it)
	{
		i++;
		if (user->get_nc_check() == 0)
		{
			if (words.size() >= 2 && words[1].compare(_password) != 0)
			{
				sendOneRPL(ERR_UNKNOWNCOMMAND(user->getNickname(), words[0]), fd);
				return (1);
			}
			if (words.size() >= 2 && words[1].compare(_password) == 0)
			{
				user->incre_nc_check();
			}
			else
			{
				sendOneRPL(ERR_PASSWDMISMATCH(user->getNickname()), fd);
				return (1);
			}
		}
		if (user->get_nc_check() == 1)
		{
			if (*it == "NICK")
			{
		        if (it + 1 != words.end()) 
				{
					std::string elementApresNick = *(it + 1); 
					if (check_nick(elementApresNick, fd, user) == 0)
					{
						user->incre_nc_check();
						continue ;
					}
					else
					{
				    	sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "NICK"), fd);
						return (1);
					}
				}
				else
				{
					sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(),"NICK"), fd);
					return (1);
				}                
		    }
		}	     
		else if (user->get_nc_check() == 2)
		{
			if (*it == "USER")
			{
				while (it != words.end())
				{
					elementsApresUser.push_back(*it);
					++it;
				}
				if (elementsApresUser.size() != 5)
				{
					sendOneRPL(ERR_NEEDMOREPARAMS(user->getNickname(), "USER"), fd);
					return (1);
				}
				else
				{
					if (check_user_nc(fd, user, elementsApresUser) != 0)
					{
						return (1);
					}
					else
					{	
						user->incre_nc_check();
						return (0);
					}
				}
			}
		}
	}
	return (0);
}