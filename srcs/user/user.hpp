/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:32:47 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/30 15:29:51 by mhajji-b         ###   ########.fr       */
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

	int &getFd();
	std::string &getNickname();
	int &get_nc_check(void);
	bool &get_check_in_server();
	std::string &Get_Error();

	void set_in_server(bool _in_server);
	void setNickname(std::string name);
	void setUsername(std::string name);
	void setRealname(std::string name);

	void setFd(int fd);
	void setIsOperator(int isOperator);
	void sendMessage(char buffer[1024]);
	void incre_nc_check(void);
	void setError(std::string error);

private:
	std::string _nickname;
	std::string _username;
	std::string _realname;
	int _fd;
	int _isOperator;
	int _check_nc;
	bool _in_server;
	std::string _error;
};
