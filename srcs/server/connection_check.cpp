/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection_check.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:09:54 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/10 14:53:55 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

bool Server::is_connected(int fd)
{
	User *user = getUserNo(fd);
	return (user->get_check_in_server());
}
