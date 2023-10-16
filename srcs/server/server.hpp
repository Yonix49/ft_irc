/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:02:05 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/16 19:33:40 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEF_SERVER
#define DEF_SERVER
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
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


private:
	int m_port;
	ServerSocket m_socket_server;
	std::string m_password;
	std::vector<int> m_vec;
	
	ServerSocket createServerSocket(int port);
};

#endif