/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:28:51 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/10 15:22:00 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include "server/server.hpp"

int		g_signal = 0;

int main(int argc, char **argv)
{
	Server &server = Server::getInstance();
	if (argc != 3)
	{
		std::cerr << "Usage: <port> <password>" << std::endl;
		return (2);
	}
	char *end_ptr;
	long int port = strtol(argv[1], &end_ptr, 10);
	server.setPort(port);
	if (*end_ptr != '\0')
		return(std::cerr << "Error: Invalid port  " << *end_ptr << std::endl, 4);
	else if (errno != 0)
		return (std::cerr << "Error: Invalid port \"" << argv[1] << "\": " << "Invalid caracter found" << std::endl, 2);
	else if ((port < 0 || port > 65535))
		return (std::cerr << "Error: Invalid port \"" << argv[1] << "\": " << "Port must be between 0 and 65535" << std::endl, 2);
	server.setPassword(std::string(argv[2]));
	if (server.createServerSocket() == -1)
		return (3);
	return 0;
}