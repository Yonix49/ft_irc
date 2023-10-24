/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:02:05 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/24 13:07:03 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEF_SERVER
# define DEF_SERVER
# include <iostream>
# include <fstream>
# include <unistd.h>
# include <string>
# include <cstdlib>
# include <cstring>
# include <string>
# include <cerrno>
# include <vector>
# include <sys/socket.h>
# include <sys/select.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/epoll.h>
# include <signal.h>

extern int			g_signal;

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
	Server& 			operator=(const Server& src);

	int 				createServerSocket();
	int 				launchSocket();
	int 				recieve_data(int fd);
	int 				newUser(int fd);

	void				setPort(long int port);


private:
	int 				_port;
	std::string			_password;
	std::vector<struct epoll_event>	_vec;
	ServerSocket		_serv;
	
};

void	signal_ctrl_c(int signo);


#endif