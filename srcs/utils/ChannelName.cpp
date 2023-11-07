/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelName.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 15:33:11 by kgezgin           #+#    #+#             */
/*   Updated: 2023/11/03 17:14:54 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes.hpp"

std::string	getChannelName(std::string cmdLine)
{
	//! parser le name
	if (cmdLine.empty() == true)
	{
		// rpl cahnnel name empty
		return (NULL);
	}
	if (cmdLine.c_str()[0] != '#')
		return ("#" + cmdLine);
	return (cmdLine);
}


std::vector<std::string> split(std::string str, std::string delimiters)
{
	std::vector<std::string>	tokens;
	std::string					ret;
	size_t						pos = 0;

	while ((pos = str.find(delimiters)) != std::string::npos){
		ret = str.substr(0, pos);
		str = str.substr(pos + 1);
		if (!ret.empty())
			tokens.push_back(ret);
	}
	if (!str.empty())
		tokens.push_back(str);
	return tokens;
}