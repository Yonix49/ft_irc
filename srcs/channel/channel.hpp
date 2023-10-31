/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:46:14 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/31 13:29:56 by kgezgin          ###   ########.fr       */
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
		std::string					&getTopic(void);
		std::string					&getPassword(void);
		int							getLimitUsers(void);
		int							getNbUsers(void);
		bool						getMode_i(void);
		bool						getMode_t(void);
		bool						getMode_k(void);
		bool						getMode_o(void);
		bool						getMode_l(void);
		void						setMode_i(bool nb);
		void						setMode_t(bool nb);
		void						setMode_k(bool nb);
		void						setMode_o(bool nb);
		void						setMode_l(bool nb);
		void						setTopic(std::string topic);
		void						setName(std::string name);
		void						setUsers(std::vector<User> users);
		void						setNbUsers(int nbUsers);
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
		bool						_mode_i;
		bool						_mode_t;
		bool						_mode_k;
		bool						_mode_o;
		bool						_mode_l;
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