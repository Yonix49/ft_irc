/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:02:05 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/25 13:25:37 by kgezgin          ###   ########.fr       */
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
		Server& 				operator=(const Server& src);
	
		int 					createServerSocket();
		int 					launchSocket();
		int 					recieve_data(int fd, int isNewUser);
		int						checkConnection(int fd, char buffer[1024]);
		int 					newUser(int fd, char buffer[1024]);
	
		void					setPort(long int port);
	
	
	private:
		int 					_port;
		ServerSocket			_serv;
		std::string				_password;
		std::vector<User>		_users;
		std::vector<Channel>	_channels;
};



#endif