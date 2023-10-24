/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:33:20 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/24 17:16:28 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_HPP
# define INCLUDES_HPP
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
# include <sstream>

extern int			g_signal;

void	signal_ctrl_c(int signo);


#endif