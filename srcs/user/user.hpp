/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:32:47 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/30 16:38:31 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../includes.hpp"

class User
{
	public:
		User();
		~User();
		User(std::string nick);
		User(const User& src);
		User& 						operator=(const User& src);

		int							&getFd();
		std::string					&getNickname();
		int							getisOperator();
		int							getisFounder();
		int							getisInvited(std::string channelName);
		void						setNickname(std::string name);
		void						setFd(int fd);
		void						setIsOperator(int isOperator);
		void						setIsFounder(int isOperator);
		// void						setChanNick();
		void						addisInvited(std::string channelName);
		void						sendMessage(char buffer[1024]);

	private:
		std::vector<std::string>	_isInvited;
		std::string					_nickname;
		std::string					_chanNick;
		int					findUser(std::string str, std::vector<User> users);
std::string					_username;
		int							_fd;
		int							_isOperator;
		int							_isFounder;
		// int							_isConfirmed;
};

int					findUser(std::string str, std::vector<User> users);
