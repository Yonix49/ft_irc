/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:28:51 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/17 11:24:01 by kgezgin          ###   ########.fr       */
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

int main(int argc, char **argv)
{
	Server	serv;
	if (argc != 3)
	{
		std::cerr << "Usage: <port> <password>" << std::endl;
		return (2);
	}
	char *end_ptr;
	long int port = strtol(argv[1], &end_ptr, 10);
	std::cout << port << std::endl;
	if (*end_ptr != '\0')
		return(std::cerr << "Error: Invalid port  " << *end_ptr << std::endl, 4);
	else if (errno != 0)
		return (std::cerr << "Error: Invalid port \"" << argv[1] << "\": " << "Invalid caracter found" << std::endl, 2);
	else if ((port < 0 || port > 65535))
		return (std::cerr << "Error: Invalid port \"" << argv[1] << "\": " << "Port must be between 0 and 65535" << std::endl, 2);
	std::string password = std::string(argv[2]);
	
	serv.createServerSocket(port);
	// std::cout << password << std::endl; 

	
	// else
	// {
	// }
	// std::cout << "Int vector containers" << std::endl;
	// std::vector <int> vec;
	// vec.push_back(1);
	// vec.push_back(2);
	// vec.push_back(3);
	// vec.push_back(4);
	// vec.push_back(5);
	return 0;
}