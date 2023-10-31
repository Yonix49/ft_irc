/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:32:47 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/31 16:03:56 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes.hpp"

class User
{
public:
	User();
	~User();
	User(std::string nick);
	User(const User &src);
	User &operator=(const User &src);
	public:
		User();
		~User();
		User(std::string nick);
		User(const User& src);
		User& 						operator=(const User& src);

	int &getFd();
	std::string &getNickname();
	int getisOperator();
	int getisFounder();
	int getisInvited(std::string channelName);
	void setNickname(std::string name);
	int &get_nc_check(void);
	void setFd(int fd);
	void setIsOperator(int isOperator);
	void setIsFounder(int isOperator);

	void setUsername(std::string name);
	void setRealname(std::string name);
	void set_in_server(bool _in_server);
	void incre_nc_check(void);
	// void						setChanNick();
	void addisInvited(std::string channelName);
	void sendMessage(char buffer[1024]);
	std::string &Get_Error();
	bool &get_check_in_server();

	void setError(std::string error);

	private:
		std::string		_nickname;
		std::string		_username;
		int				_fd;
		int				_isOperator;

};
