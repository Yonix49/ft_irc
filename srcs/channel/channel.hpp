/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:46:14 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/30 16:38:37 by kgezgin          ###   ########.fr       */
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
		std::string					getListUsers(void);
		std::string					getTopic(void);
		std::string					getPassword(void);
		int							getLimitUsers(void);
		int							getNbUsers(void);
		bool						getIsInviteOnlyMod(void);
		void						setTopic(std::string topic);
		void						setName(std::string name);
		void						setUsers(std::vector<User> users);
		void						setNbUsers(int nbUsers);
		void						setIsInviteOnlyMod(bool nb);
		void						setIsOperator(std::vector<User> operators);
		void						setLimitUsers(int limit);
		void						setPassword(std::string password);

		// void						kickUser(User);
		void						addInvitedUser(std::string nickname);
		void						updateInvited(std::string nickname);
		int							addUser(User user, int isOperator, std::string channelName, int fd);
		int							isUserinchan(std::string nickname, int type);
		void						sendRPLtoChan(std::string rpl);

	private:
		bool						_isInviteOnlyMod;
		int							_limitUsers;
		int							_nbUsers;
		std::string 				_name;
		std::string 				_password;
		std::string 				_topic;
		std::vector<std::string>	_invited;
		std::vector<User>			_users;
		std::vector<User>			_operators;
};

int					findUser(std::string str, std::vector<User> users);


#endif