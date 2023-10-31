/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:02:05 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/31 16:02:02 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include "../includes.hpp"
# include "../channel/channel.hpp"
// # include "../user/user.hpp"
#include <map>
#include <functional>
#include <string>
#include <exception>
#include <stdexcept> // Pour les exceptions standard (ceci peut dépendre de votre utilisation)

struct ServerSocket
{
	struct sockaddr_in	serverAddress;
	struct sockaddr_in	clientAddress;
	socklen_t			clientAddrLen;
	int					clientSocket;
	int					serverSocket;
	int					epollFd;  // Crée un descripteur de fichier epoll
};

typedef void (*CommandFunction)(std::string, int);
void sendOneRPL(std::string rpl, int fd);

class Server
{
public:
	// Constructeur
	static Server &getInstance()
	{
		static Server instance;
		return instance;
	}

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

		int						launchCmd(char buffer[1024], int fd);
		std::vector<std::string> get_cmdLine(char buffer[1024]);

		int						join(std::vector<std::string> cmdLine, int fd);
		int						channelExist(std::string channelName);

		int						invite(std::vector<std::string> cmdLine, int fd);

		int						topic(std::vector<std::string> cmdLine, int fd);	

		int						mode(std::vector<std::string> cmdLine, int fd);
		int						mode_l(std::vector<std::string> cmdLine, int i);
		int						mode_k(std::vector<std::string> cmdLine, int i, int fd, User *user);
		int						mode_i(int i);

		int						invite(std::vector<std::string> cmdLine, int fd);

		int						topic(std::vector<std::string> cmdLine, int fd);	

		int						mode(std::vector<std::string> cmdLine, int fd);
		int						mode_l(std::vector<std::string> cmdLine, int i);
		int						mode_k(std::vector<std::string> cmdLine, int i, int fd, User *user);
		int						mode_i(int i);

	int check_nickname(std::vector<std::string> str);
	int check_password(std::vector<std::string> str);
	int nc_check(std::string str, int fd);
	int irssi_check(std::string str, int fd);
	void initialize_command_map();
	std::vector<std::string> get_vector_ref(std::string str);
	int irsii_argument_check(std::vector<std::string> words, int fd, User *user);
	int check_user_irsi(int fd, User *user, std::vector<std::string> words);

	// containers map
	void addCommand(const std::string &command, CommandFunction function);
	void initializeCommandMap();
	int use_map_function(std::string buffer, int fd);
	static void HandleNickCommand(std::string param, int fd);
	static void HandleUserCommand(std::string param, int fd);
	static void HandlePassCommand(std::string param, int fd);
	void Set_error_message(void);

	std::string get_password(void);
	void setPassword(std::string password);
	void setPort(long int port);
	User *getUserNo(int fd);
	int check_nick(std::string nickname, int fd, User *user);
	int check_user_nc(int fd, User *user, std::vector<std::string> words);
	int ft_lauch_commmand(int fd, std::string str);
	bool is_connected(int fd);
	void set_Error_user(std::string error, int fd);
	std::string get_Error_user(int fd);

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
		int 					_port;
		ServerSocket			_serv;
		std::string				_password;
		std::vector<User>		_users;
		std::vector<Channel *>	_channels;
};

int					findUser(std::string str, std::vector<User> users);

#endif