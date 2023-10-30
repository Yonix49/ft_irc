/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelName.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 15:33:11 by kgezgin           #+#    #+#             */
/*   Updated: 2023/10/30 15:15:15 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes.hpp"

std::string	getChannelName(std::string cmdLine)
{
	// parser le name
	if (cmdLine.empty() == true)
	{
		// rpl cahnnel name empty
		return (NULL);
	}
	if (cmdLine.c_str()[0] != '#')
		return ("#" + cmdLine);
	return (cmdLine);
}

