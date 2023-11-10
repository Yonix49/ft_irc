/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 18:04:18 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/10 12:21:18 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
{
	_serv.clientSocket = 0;
	_serv.serverSocket = 0;
}

Server::~Server()
{
}
// Constructeur
// Server::Server()
// {
// 	_checking_nc = 0;
// }
Server::Server(int port, const std::string &password)
{
	_port = port;
	_password = password;
	_serv.clientSocket = 0;
	_serv.serverSocket = 0;
	_checking_nc = 0;
}

// Constructeur de copie
Server::Server(const Server &src)
{
	_password = src._password;
	// _vec = src._vec;
	_serv = src._serv;
	_port = src._port;
}

// Opérateur d'assignation
Server &Server::operator=(const Server &src)
{
	if (this != &src)
	{
		this->_password = src._password;
		// this->_vec = src._vec;
		this->_serv = src._serv;
		this->_port = src._port;
	}
	return *this;
}
//---------------------SET_VAR---------------------------///////

void Server::setPort(long int port)
{
	_port = port;
}

void Server::setPassword(std::string password)
{
	_password = password;
}
//---------------------GETTEUR---------------------------///////

User *Server::getUserNo(int fd)
{
	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->getFd() == fd)
			return &(*it);
	}
	return (NULL);
}

int findUser(std::string str, std::vector<User> users)
{
	int i = 0;
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (!str.compare((*it).getNickname()))
			return (i);
		i++;
	}
	return (-1);
}

User *Server::getUserString(std::string nickname)
{
	for (std::vector<User>::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (!it->getNickname().compare(nickname))
			return &(*it);
	}
	return (NULL);
}

std::vector<std::string> Server::get_vector_ref(const std::string &str)
{
	std::istringstream iss(str); // Utiliser un std::istringstream pour diviser en mots
	std::vector<std::string> words;

	std::string word;
	while (iss >> word)
	{
		words.push_back(word);
		// std::cout << "ici" << word << std::endl;
	}
	return (words);
}
std::string Server::get_password(void)
{
	return (_password);
}

void	Server::setEvent(struct epoll_event	event)
{
	_serv.event = event;
}

void Server::set_Error_user(std::string error, int fd)
{
	User *user = NULL; // Déclarer un pointeur vers un utilisateur
	user = getUserNo(fd);
	user->setError(error);
}

std::string Server::get_Error_user(int fd)
{
	User *user = NULL; // Déclarer un pointeur vers un utilisateur
	user = getUserNo(fd);
	if (!user)
		return (NULL);
	return (user->Get_Error());
}

//---------------------GETTEUR---------------------------///////
int Server::createServerSocket()
{
	int numEvents;
	struct epoll_event event;
	struct epoll_event events[10]; // Nombre maximal d'événements à gérer
	int fd;
	int isNewUser;

	_serv.epollFd = epoll_create(1);
	initializeCommandMap();
	if (_serv.epollFd == -1)
	{
		std::cerr << "Erreur lors de la création d'epoll." << std::endl;
		return 1;
	}
	launchSocket();

	event.events = EPOLLIN;
	event.data.fd = _serv.serverSocket;
	if (epoll_ctl(_serv.epollFd, EPOLL_CTL_ADD, _serv.serverSocket, &event) == -1)
	{
		std::cerr << "Erreur lors de l'ajout du socket du serveur à epoll." << std::endl;
		return 1;
	}
	signal(SIGINT, signal_ctrl_c);
	isNewUser = 0;
	while (!g_signal)
	{
		numEvents = epoll_wait(_serv.epollFd, events, 10, -1);
		if (numEvents == -1)
		{
			if (!g_signal)
				std::cerr << "Erreur lors de l'appel à epoll_wait." << std::endl;
			else
				std::cerr << "CNTRL+C detected (epoll_wait)." << std::endl;
			for (std::vector<Channel *>::iterator it = _channels.begin(); it < _channels.end(); ++it)
				delete *it;
			_channels.clear();
			for (std::vector<User>::iterator it2 = _users.begin(); it2 < _users.end(); ++it2)
				close(it2->getFd());
			close(_serv.clientSocket);
			return 1;
		}
		for (int i = 0; i < numEvents; i++)
		{
			fd = events[i].data.fd;
			if (fd == _serv.serverSocket)
			{
				_serv.clientSocket = accept(_serv.serverSocket, (struct sockaddr *)&_serv.clientAddress, &_serv.clientAddrLen);
				if (_serv.clientSocket == -1)
					std::cerr << "Erreur lors de l'acceptation de la connexion entrante." << std::endl;
				else
				{
					newUser(_serv.clientSocket, NULL);
					isNewUser = 1;
				}
			}
			else
			{
				setEvent(event);
				recieve_data(fd, isNewUser);
				isNewUser = 0;
			}
		}
	}
	close(_serv.clientSocket);
	return (_serv.serverSocket);
}


int Server::recieve_data(int fd, int isNewUser)
{
    int bytesRead;
    char buffer[1024];
    int flag = 0;
    bytesRead = recv(fd, buffer, sizeof(buffer), 0);
    if (bytesRead < 0)
        std::cerr << "Erreur lors de la réception de données du client." << std::endl;
    else if (bytesRead == 0 || strncmp(buffer, "QUIT", 4) == 0)
    {
        std::cout << "User disconnected" << std::endl;
        quit(buffer, fd);
        close(fd);
        return -1;
    }

    buffer[bytesRead] = '\0';
    std::string str(buffer);
    while (1)
    {
        if (str[0] == '\0' || str[str.length() - 1] == '\n')
        {
            break;
        }
        else
        {
            flag++;
            bytesRead = recv(fd, buffer, sizeof(buffer), 0);
            if (bytesRead < 0)
            {
                std::cerr << "Erreur lors de la réception de données du client." << std::endl;
            }
            else if (bytesRead == 0 || strncmp(buffer, "QUIT", 4) == 0)
            {
                std::cout << "User disconnected" << std::endl;
                quit(buffer, fd);
                close(fd);
                return -1;
            }
            buffer[bytesRead] = '\0';
            str += std::string(buffer); 
        }
    }

    if (!str.empty() && is_connected(fd) == false)
    {
        if (str.length() >= 2 && is_connected(fd) == false)
        {
            std::string lastTwoChars = str.substr(str.length() - 2, 2);
            if (is_connected(fd) == false && lastTwoChars == "\r\n")
            {
                for (size_t i = 0; i < str.length(); i++) 
                {
                    if (buffer[i] == '\r' && str[i + 1] == '\n')
                    {
                        buffer[i] = ' ';
                        buffer[i + 1] = ' ';
                    }
                }
                irssi_check(buffer, fd);
                if (is_connected(fd) == true)
                {
                    std::string welcomeMessage = RPL_WELCOME(_users.back().getNickname());
                    size_t messageLength = welcomeMessage.length();
                    if (send(fd, welcomeMessage.c_str(), messageLength, 0) == -1)
                    {
                        exit (0);
                        std::cout << "error envoie RPL" << std::endl;
                    }
                }
            }
            else
            {
                nc_check(str.c_str(), fd, flag);
                if (is_connected(fd) == true)
                {
                    std::string welcomeMessage = RPL_WELCOME(_users.back().getNickname());
                    size_t messageLength = welcomeMessage.length();
                    if (send(fd, welcomeMessage.c_str(), messageLength, 0) == -1)
                        std::cout << "error envoie RPL" << std::endl;
                }
            }
        }
    }
    else if (isNewUser == 0 && is_connected(fd) == true)
    {
        use_map_function(str, fd);
    }
    return (0);
}

int Server::newUser(int fd, char buffer[1024])
{
	(void)buffer;
	struct epoll_event clientEvent;
	int bytesSent;
	(void)bytesSent;
	User newUser("");
	newUser.setFd(fd);
	_users.push_back(newUser);
	clientEvent.events = EPOLLIN;
	clientEvent.data.fd = _serv.clientSocket;
	if (epoll_ctl(_serv.epollFd, EPOLL_CTL_ADD, _serv.clientSocket, &clientEvent) == -1)
	{
		std::cerr << "Erreur lors de l'ajout du socket client à epoll." << std::endl;
		return 1;
	}
	return 0;
}

void	Server::rmChannel(Channel Chan)
{
	for (std::vector<Channel *>::iterator it = _channels.begin(); it < _channels.end(); ++it)
	{
		if (!(*it)->getName().compare(Chan.getName()))
		{
			_channels.erase(it);
			std::cout << "le channel " << (*it)->getName() << " est supprimer" << std::endl;
			return ;
		}
	}
}


int Server::launchSocket()
{
	int option = 1;
	_serv.serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serv.serverSocket == -1)
	{
		std::cerr << "error socket" << std::endl;
		return 1;
	}
	_serv.serverAddress.sin_family = AF_INET;
	_serv.serverAddress.sin_addr.s_addr = INADDR_ANY;
	_serv.serverAddress.sin_port = htons(_port);
	if (setsockopt(_serv.serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) < 0)
		throw std::runtime_error("Setsockopt failed");
	if (bind(_serv.serverSocket, (struct sockaddr *)&_serv.serverAddress, sizeof(_serv.serverAddress)) == -1)
	{
		std::cerr << "Erreur lors de la liaison de la socket à l'adresse et au port." << std::endl;
		return 1;
	}
	_serv.clientAddrLen = sizeof(_serv.clientAddress);
	if (listen(_serv.serverSocket, 10) == -1)
	{
		std::cerr << "Erreur lors de la mise en écoute de la socket." << std::endl;
		return 1;
	}
	std::cout << "Server socket ready : " << _serv.serverSocket << std::endl;
	return (0);
}

//! fermer tout les fd !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ! Momo: No such channel lors de la connection