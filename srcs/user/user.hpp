/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:32:47 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/25 16:45:21 by kgezgin          ###   ########.fr       */
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
		User& 			operator=(const User& src);

		int				&getFd();
		std::string		&getNickname();
		void			setNickname(std::string name);
		void			setFd(int fd);
		void			setIsOperator(int isOperator);
		void			sendMessage(char buffer[1024]);

	private:
		std::string		_nickname;
		std::string		_username;
		int				_fd;
		int				_isOperator;

};
