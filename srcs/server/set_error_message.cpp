/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_error_message.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgezgin <kgezgin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:58:43 by mhajji-b          #+#    #+#             */
/*   Updated: 2023/11/10 10:37:21 by kgezgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
// void Server::Set_error_message(void)
// {

// 	std::map<int, std::string> errorMessages = {
// 		{472, ERR_UNKNOWNMODE(nickname, mode)},
// 		{474, ERR_BANNEDFROMCHAN(client, channel)},
// 		{481, ERR_NOPRIVILEGES(nickname)},
// 		{482, ERR_CHANOPRIVSNEED(nickname, chanel)},
// 		{501, ERR_UMODEUNKNOWNFLAG(nickname)},
// 		{464, ERR_PASSWDMISMATCH(nickname)},
// 		{311, RPL_WHOISUSER(nickname, other_nick, username)},
// 		{312, RPL_WHOISSERVER(nickname, other_nick)},
// 		{315, RPL_ENDOFWHO(nickname, chanel)},
// 		{317, RPL_WHOISIDLE(nickname, other_nick, create_time)},
// 		{318, RPL_ENDOFWHOIS(nickname, other_nick)},
// 		{352, RPL_WHOREPLY(nickname, chanel, username, concerned_client_nickname, status, mode)},
// 		{352, RPL_WHOREPLY_BIS(nickname, chanel, username, concerned_client_nickname, status)},
// 		{369, RPL_ENDOFWHOWAS(nickname, other_nick)},
// 		{321, RPL_LISTSTART(nickname)},
// 		{322, RPL_LIST(nickname, chanel, usercount)},
// 		{323, RPL_LISTEND(nickname)},
// 		{401, ERR_NOSUCHNICK(nickname, other_nick)},
// 		{406, ERR_WASNOSUCHNICK(nickname, other_nick)},
// 		{433, ERR_NICKNAMEINUSE(nickname)},
// 		{421, ERR_UNKNOWNCOMMAND(nickname, cmd)},
// 		{461, ERR_NEEDMOREPARAMS(nickname, cmd)},
// 		{513, ERR_BADPING(nickname)},
// 		{403, ERR_NOSUCHCHANNEL(nickname, chanel)},
// 		{404, ERR_CANNOTSENDTOCHAN(nickname, chanel)},
// 		{441, ERR_USERNOTINCHANNEL(nickname, concerned_client_nickname, chanel)},
// 		{442, ERR_NOTONCHANNEL(nickname, chanel)},
// 		{473, ERR_INVITEONLYCHAN(nickname, chanel)}
// 		// Ajoutez d'autres correspondances d'erreur ici
// 	};
// }