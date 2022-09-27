/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 11:05:18 by hel-makh          #+#    #+#             */
/*   Updated: 2022/09/27 13:14:39 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "replies.hpp"

std::string	reply(int repl_no) {
	switch (repl_no) {
		case RPL_WHOISOPERATOR:
			return(":is an IRC operator");
		case RPL_ENDOFWHOIS:
			return(":End of /WHOIS list");
		case RPL_LISTSTART:
			return("Channel :Users  Name");
		case RPL_ENDOFWHOWAS:
			return(":End of WHOWAS");
		case RPL_LISTEND:
			return(":End of /LIST");
		case RPL_NOTOPIC:
			return(":No topic is set");
		case RPL_ENDOFWHO:
			return(":End of /WHO list");
		case RPL_ENDOFNAMES:
			return(":End of /NAMES list");
		case RPL_ENDOFBANLIST:
			return(":End of channel ban list");
		case RPL_ENDOFINFO:
			return(":End of /INFO list");
		case RPL_YOUREOPER:
			return(":You are now an IRC operator");
		case RPL_ADMINME:
			return(":Administrative info");
		default:
			return(":Undefined reply");
	}
}

std::string	err_reply(int err_no) {
	switch (err_no) {
		case ERR_NOSUCHNICK:
			return(":No such nick/channel");
		case ERR_NOSUCHSERVER:
			return(":No such server");
		case ERR_NOSUCHCHANNEL:
			return(":No such channel");
		case ERR_CANNOTSENDTOCHAN:
			return(":Cannot send to channel");
		case ERR_TOOMANYCHANNELS:
			return(":You have joined too many channels");
		case ERR_WASNOSUCHNICK:
			return(":There was no such nickname");
		case ERR_TOOMANYTARGETS:
			return(":Duplicate recipients. No message delivered");
		case ERR_NOORIGIN:
			return(":No origin specified");
		case ERR_NORECIPIENT:
			return(":No recipient given");
		case ERR_NOTEXTTOSEND:
			return(":No text to send");
		case ERR_NOTOPLEVEL:
			return(":No toplevel domain specified");
		case ERR_WILDTOPLEVEL:
			return(":Wildcard in toplevel domain");
		case ERR_UNKNOWNCOMMAND:
			return(":Unknown command");
		case ERR_NOMOTD:
			return(":MOTD File is missing");
		case ERR_NOADMININFO:
			return(":No administrative info available");
		case ERR_FILEERROR:
			return(":File error doing <file op> on <file>");
		case ERR_NONICKNAMEGIVEN:
			return(":No nickname given");
		case ERR_ERRONEUSNICKNAME:
			return(":Erroneus nickname");
		case ERR_NICKNAMEINUSE:
			return(":Nickname is already in use");
		case ERR_NICKCOLLISION:
			return(":Nickname collision KILL");
		case ERR_USERNOTINCHANNEL:
			return(":They aren't on that channel");
		case ERR_NOTONCHANNEL:
			return(":You're not on that channel");
		case ERR_USERONCHANNEL:
			return(":is already on channel");
		case ERR_NOLOGIN:
			return(":User not logged in");
		case ERR_USERSDISABLED:
			return(":USERS has been disabled");
		case ERR_NOTREGISTERED:
			return(":You have not registered");
		case ERR_NEEDMOREPARAMS:
			return(":Not enough parameters");
		case ERR_ALREADYREGISTRED:
			return(":You may not reregister");
		case ERR_NOPERMFORHOST:
			return(":Your host isn't among the privileged");
		case ERR_PASSWDMISMATCH:
			return(":Password incorrect");
		case ERR_YOUREBANNEDCREEP:
			return(":You are banned from this server");
		case ERR_KEYSET:
			return(":Channel key already set");
		case ERR_CHANNELISFULL:
			return(":Cannot join channel (+l)");
		case ERR_UNKNOWNMODE:
			return(":is unknown mode char to me");
		case ERR_INVITEONLYCHAN:
			return(":Cannot join channel (+i)");
		case ERR_BANNEDFROMCHAN:
			return(":Cannot join channel (+b)");
		case ERR_BADCHANNELKEY:
			return(":Cannot join channel (+k)");
		case ERR_NOPRIVILEGES:
			return(":Permission Denied- You're not an IRC operator");
		case ERR_CHANOPRIVSNEEDED:
			return(":You're not channel operator");
		case ERR_NOOPERHOST:
			return(":No O-lines for your host");
		case ERR_UMODEUNKNOWNFLAG:
			return(":Unknown MODE flag");
		case ERR_USERSDONTMATCH:
			return(":Cant change mode for other users");
		default:
			return(":Undefined error");
	}
}
