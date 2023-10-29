/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection_check.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 16:09:54 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/10/29 16:10:43 by mhajji-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

bool Server::is_connected(int fd)//Check la connextion via check var
{
	User *user = NULL; // Déclarer un pointeur vers un utilisateur
	user = getUserNo(fd);

	return (user->get_check_in_server());
}