/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:32:47 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/07 11:12:52 by kgezgin          ###   ########.fr       */
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
	User						&operator=(const User &src);

	std::string					&getNickname();
	std::string					&getUsername();
	int							&getFd();
	int							&getisOperator();
	int							&getisFounder();
	int							&getIsKick();
	int							getisInvited(std::string channelName);
	bool						&get_check_in_server();
	bool						&getInvisible();
	std::string					&Get_Error();
	int							&get_nc_check(void);
	int							&getHasQuit(void);
	void						setFd(int fd);
	void						setIsKick(int fd);
	void						setHasQuit(int hasQuit);
	void						setNickname(std::string name);
	void						setIsOperator(int isOperator);
	void						setIsFounder(int isOperator);
	void						setUsername(std::string name);
	void						setRealname(std::string name);
	void						set_in_server(bool _in_server);
	void						setInvisible(bool _in_server);
	void						setError(std::string error);

	void						incre_nc_check(void);
	// void						setChanNick();
	void						addisInvited(std::string channelName);
	void						sendMessage(char buffer[1024]);




private:
	std::vector<std::string>	_isInvited;
	std::string 				_nickname;
	std::string 				_username;
	std::string 				_realname;
	int							_fd;
	int							_isOperator;
	int							_isFounder;
	int							_check_nc;
	bool						_in_server;
	bool						_invisible;
	std::string					_error;
	int							_hasQuit;
	int							_isKick;
};

