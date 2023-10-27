/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:02:05 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/27 20:15:06 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "../includes.hpp"
#include "../channel/channel.hpp"
// # include "../user/user.hpp"
#include <map>
struct ServerSocket
{
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	socklen_t clientAddrLen;
	int clientSocket;
	int serverSocket;
	int epollFd; // Crée un descripteur de fichier epoll
};
typedef void (*CommandFunction)(const std::string &, int);

class Server
{
public:
	// Constructeur
	Server();
	~Server();
	Server(int port, const std::string &password);
	Server(const Server &src);
	// operateur =
	Server &operator=(const Server &src);

	int createServerSocket();
	int launchSocket();
	int recieve_data(int fd, int isNewUser);
	int checkConnection(int fd, char buffer[1024]);
	int newUser(int fd, char buffer[1024]);

	int launchCmd(char buffer[1024], int fd);
	std::vector<std::string> get_cmdLine(char buffer[1024]);

	int join(std::vector<std::string> cmdLine, int fd);
	int channelExist(std::string channelName);

	int check_nickname(std::vector<std::string> str);
	int check_password(std::vector<std::string> str);
	int nc_check(std::string str, int fd);
	int irssi_check(std::string str, int fd);
	void initialize_command_map();
	std::vector<std::string> get_vector_ref(std::string str);
	int irsii_argument_check(std::vector<std::string> words, int fd, User *user);
	int check_user_irsi(int fd, User *user, std::vector<std::string> words);
	static void HandleNickCommand(const std::string &param, int fd);

	// containers map
	void addCommand(const std::string &command, CommandFunction function);
	void initializeCommandMap();
	int use_map_function(char buffer[1024], int fd);

	std::string get_password(void);
	void setPassword(std::string password);
	void setPort(long int port);
	User *getUserNo(int fd);
	int check_nick(std::string nickname, int fd, User *user);
	int check_user_nc(int fd, User *user, std::vector<std::string> words);
	int ft_lauch_commmand(int fd, std::string str);
	bool is_connected(int fd);
private:
	int _port;
	ServerSocket _serv;
	std::string _password;
	std::vector<User> _users;
	std::vector<Channel> _channels;
	int _checking_nc;

	// std::map<std::string, CommandFunction> commandMap;
};

#endif