/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:02:05 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/02 21:16:14 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include "../includes.hpp"
#include "../channel/channel.hpp"
// # include "../user/user.hpp"
#include <map>
#include <functional>
#include <string>
#include <exception>
#include <stdexcept> // Pour les exceptions standard (ceci peut dépendre de votre utilisation)

struct ServerSocket
{
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
	socklen_t clientAddrLen;
	int clientSocket;
	int serverSocket;
	int epollFd; // Crée un descripteur de fichier epoll
};

typedef void (*CommandFunction)(std::string, int);
void sendOneRPL(std::string rpl, int fd);
std::vector<std::string> split(std::string str, std::string delimiters);

class Server
{
public:
	// Constructeur
	static Server &getInstance()
	{
		static Server instance;
		return instance;
	}
	Server();
	~Server();
	Server(int port, const std::string &password);
	Server(const Server &src);
	// operateur =
	Server &operator=(const Server &src);
	static void					Set_error_message(void);
	void						setPassword(std::string password);
	void						setPort(long int port);
	void						set_Error_user(std::string error, int fd);
	std::string					get_password(void);
	std::string					get_Error_user(int fd);
	User						*getUserNo(int fd);

	int							createServerSocket();
	int							launchSocket();
	int							recieve_data(int fd, int isNewUser);
	int							checkConnection(int fd, char buffer[1024]);
	int							newUser(int fd, char buffer[1024]);

	std::vector<std::string>	get_cmdLine(char buffer[1024]);

	int							channelExist(std::string channelName);
	void						rmChannel(Channel chan);

	int 						message_notice(std::vector<std::string> words, int fd, User *user, std::string param);
	static void 				HandleNoticeMessage(std::string param, int fd);

	int							message_user(std::vector<std::string> words, int fd, User *user, std::string param);
	static void					HandlePrivMessage(std::string param, int fd);
	static void					join(std::string param, int fd);
	static void					invite(std::string param, int fd);
	static void					topic(std::string param, int fd);
	static void					part(std::string param, int fd);
	static void					mode(std::string param, int fd);

	int							mode_o(std::vector<std::string> cmdLine, int i, int fd, User *user);
	int							mode_t(std::vector<std::string> cmdLine, int i, User *user);
	int							mode_l(std::vector<std::string> cmdLine, int i, User *user);
	int							mode_k(std::vector<std::string> cmdLine, int i, int fd, User *user);
	int							mode_i(std::vector<std::string> cmdLine, int i);

	int							check_nickname(std::vector<std::string> str);
	int							check_password(std::vector<std::string> str);
	int							nc_check(std::string str, int fd);
	int							irssi_check(std::string str, int fd);
	void						initialize_command_map();
	std::vector<std::string>	get_vector_ref(std::string str);
	int							irsii_argument_check(std::vector<std::string> words, int fd, User *user);
	int							check_user_irsi(int fd, User *user, std::vector<std::string> words);
	User						*getUserString(std::string nickname);

	// containers map
	void						addCommand(const std::string &command, CommandFunction function);
	void						initializeCommandMap();
	int							use_map_function(std::string buffer, int fd);
	static void					HandleNickCommand(std::string param, int fd);
	static void					HandleUserCommand(std::string param, int fd);
	static void					HandlePassCommand(std::string param, int fd);

	int 						Same_Nick(std::string nickname, int fd, User *user);
	int							check_nick(std::string nickname, int fd, User *user);
	int							check_user_nc(int fd, User *user, std::vector<std::string> words);
	int							ft_lauch_commmand(int fd, std::string str);
	bool						is_connected(int fd);

	class Error_rpl : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return ("Error");
			// return errorMessage.c_str();
		}
	};

private:
	int									_port;
	ServerSocket						_serv;
	std::string 						_password;
	std::vector<User>					_users;
	std::vector<Channel *>				_channels;
	int									_checking_nc;
	User								*_userTemp;
	std::string _error;
	std::map<std::string, CommandFunction> commandMap;
	std::map<int, std::string> errorMessages;
};

int findUser(std::string str, std::vector<User> users);

#endif