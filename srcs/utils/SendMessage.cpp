/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:21:43 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/26 16:24:27 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.hpp"

void	sendOneRPL(std::string rpl, int fd)
{
	if (send(fd, rpl.c_str(), rpl.size(), 0) == -1)
		std::cerr << "Error while sending RPL" << std::endl;
}