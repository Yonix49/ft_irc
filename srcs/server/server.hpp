/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:02:05 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/20 12:44:56 by kgezgin          ###   ########.fr       */
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

struct ServerSocket
{
	int socketDescriptor;  // Le descripteur de socket
	std::string ipAddress; // L'adresse IP sur laquelle le serveur écoute
	int port;			   // Le numéro de port sur lequel le serveur écoute
						   // Autres informations, si nécessaires
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
	Server& operator=(const Server& src);
	int createServerSocket(int port);


private:
	int m_port;
	ServerSocket m_socket_server;
	std::string m_password;
	std::vector<int> m_vec;
};

#endif