/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:02:05 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/30 16:38:26 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include "../includes.hpp"
# include "../channel/channel.hpp"
// # include "../user/user.hpp"


struct ServerSocket
{
	struct sockaddr_in	serverAddress;
	struct sockaddr_in	clientAddress;
	socklen_t			clientAddrLen;
	int					clientSocket;
	int					serverSocket;
	int					epollFd;  // Crée un descripteur de fichier epoll
};

class Server
{
	public:
		//Constructeur
		Server();
		~Server();
		Server(int port, const std::string &password);
		Server(const Server& src);
		//operateur = 
		Server& 					operator=(const Server& src);
	
		int 					createServerSocket();
		int 					launchSocket();
		int 					recieve_data(int fd, int isNewUser, struct	epoll_event clientEvent);
		int						checkConnection(int fd, char buffer[1024]);
		int 					newUser(int fd, char buffer[1024]);

		int						launchCmd(char buffer[1024], int fd);
		std::vector<std::string> get_cmdLine(char buffer[1024]);

		int						join(std::vector<std::string> cmdLine, int fd);
		int						channelExist(std::string channelName);

		int						invite(std::vector<std::string> cmdLine, int fd);

		int						mode(std::vector<std::string> cmdLine, int fd);

		int 					check_nickname(std::vector<std::string> str);
		int 					check_password(std::vector<std::string> str);


		void					setPort(long int port);
		User					*getUserNo(int fd);
		User					*getUserString(std::string nickname);
	
	private:
		int 					_port;
		ServerSocket			_serv;
		std::string				_password;
		std::vector<User>		_users;
		std::vector<Channel *>	_channels;
};

int					findUser(std::string str, std::vector<User> users);


#endif