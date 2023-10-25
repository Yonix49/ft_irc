/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:46:14 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/25 17:02:33 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "../includes.hpp"
# include "../user/user.hpp"

class Channel
{
	public:
		Channel();
		Channel(std::string name);
		Channel(const Channel& src);
		~Channel();
		Channel& 					operator=(const Channel& src);

		std::string					&getName(void);
		std::string					getTopic(void);
		void						setTopic(std::string topic);
		void						setName(std::string name);
		void						setUsers(std::vector<User> users);
		void						setIsOperator(std::vector<User> operators);

		// void						kickUser(User);
		void						addUser(User user);
		void						sendMessages(char buffer[1024]);

	private:
		std::string 				_name;
		std::string 				_topic;
		std::vector<User>			_users;
		std::vector<User>			_operators;
};

#endif