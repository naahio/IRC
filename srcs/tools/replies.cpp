/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 11:05:18 by hel-makh          #+#    #+#             */
/*   Updated: 2022/09/03 13:09:08 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "replies.hpp"

std::string	reply(int repl_no, std::string msg) {
	switch (repl_no) {
		case RPL_WHOISOPERATOR:
			return(msg + " :is an IRC operator\n");
		case RPL_ENDOFWHOIS:
			return(msg + " :End of /WHOIS list\n");
		case RPL_ENDOFWHOWAS:
			return(msg + " :End of WHOWAS\n");
		case RPL_LISTEND:
			return(msg + " :End of /LIST\n");
		case RPL_NOTOPIC:
			return(msg + " :No topic is set\n");
		case RPL_ENDOFWHO:
			return(msg + " :End of /WHO list\n");
		case RPL_ENDOFNAMES:
			return(msg + " :End of /NAMES list\n");
		case RPL_ENDOFINFO:
			return(msg + " :End of /INFO list\n");
		case RPL_YOUREOPER:
			return(msg + " :You are now an IRC operator\n");
		default:
			return(msg + " :Undefined reply\n");
	}
}

std::string	err_reply(int err_no, std::string msg) {
	switch (err_no) {
		case ERR_NOSUCHNICK:
			return(msg + " :No such nick/channel\n");
		case ERR_NOSUCHSERVER:
			return(msg + " :No such server\n");
		case ERR_NOSUCHCHANNEL:
			return(msg + " :No such channel\n");
		case ERR_CANNOTSENDTOCHAN:
			return(msg + " :Cannot send to channel\n");
		case ERR_TOOMANYCHANNELS:
			return(msg + " :You have joined too many channels\n");
		case ERR_WASNOSUCHNICK:
			return(msg + " :There was no such nickname\n");
		case ERR_TOOMANYTARGETS:
			return(msg + " :Duplicate recipients. No message delivered\n");
		case ERR_NOORIGIN:
			return(msg + " :No origin specified\n");
		case ERR_NORECIPIENT:
			return(msg + " :No recipient given\n");
		case ERR_NOTEXTTOSEND:
			return(msg + " :No text to send\n");
		case ERR_NOTOPLEVEL:
			return(msg + " :No toplevel domain specified\n");
		case ERR_WILDTOPLEVEL:
			return(msg + " :Wildcard in toplevel domain\n");
		case ERR_UNKNOWNCOMMAND:
			return(msg + " :Unknown command\n");
		case ERR_NOMOTD:
			return(msg + " :MOTD File is missing\n");
		case ERR_NOADMININFO:
			return(msg + " :No administrative info available\n");
		case ERR_FILEERROR:
			return(msg + " :File error doing <file op> on <file>\n");
		case ERR_NONICKNAMEGIVEN:
			return(msg + " :No nickname given\n");
		case ERR_ERRONEUSNICKNAME:
			return(msg + " :Erroneus nickname\n");
		case ERR_NICKNAMEINUSE:
			return(msg + " :Nickname is already in use\n");
		case ERR_NICKCOLLISION:
			return(msg + " :Nickname collision KILL\n");
		case ERR_USERNOTINCHANNEL:
			return(msg + " :They aren't on that channel\n");
		case ERR_NOTONCHANNEL:
			return(msg + " :You're not on that channel\n");
		case ERR_USERONCHANNEL:
			return(msg + " :is already on channel\n");
		case ERR_NOLOGIN:
			return(msg + " :User not logged in\n");
		case ERR_USERSDISABLED:
			return(msg + " :USERS has been disabled\n");
		case ERR_NOTREGISTERED:
			return(msg + " :You have not registered\n");
		case ERR_NEEDMOREPARAMS:
			return(msg + " :Not enough parameters\n");
		case ERR_ALREADYREGISTRED:
			return(msg + " :You may not reregister\n");
		case ERR_NOPERMFORHOST:
			return(msg + " :Your host isn't among the privileged\n");
		case ERR_PASSWDMISMATCH:
			return(msg + " :Password incorrect\n");
		case ERR_YOUREBANNEDCREEP:
			return(msg + " :You are banned from this server\n");
		case ERR_KEYSET:
			return(msg + " :Channel key already set\n");
		case ERR_CHANNELISFULL:
			return(msg + " :Cannot join channel (+l)\n");
		case ERR_UNKNOWNMODE:
			return(msg + " :is unknown mode char to me\n");
		case ERR_INVITEONLYCHAN:
			return(msg + " :Cannot join channel (+i)\n");
		case ERR_BANNEDFROMCHAN:
			return(msg + " :Cannot join channel (+b)\n");
		case ERR_BADCHANNELKEY:
			return(msg + " :Cannot join channel (+k)\n");
		case ERR_NOPRIVILEGES:
			return(msg + " :Permission Denied- You're not an IRC operator\n");
		case ERR_CHANOPRIVSNEEDED:
			return(msg + " :You're not channel operator\n");
		case ERR_NOOPERHOST:
			return(msg + " :No O-lines for your host\n");
		case ERR_UMODEUNKNOWNFLAG:
			return(msg + " :Unknown MODE flag\n");
		case ERR_USERSDONTMATCH:
			return(msg + " :Cant change mode for other users\n");
		default:
			return(msg + " :Undefined error\n");
	}
}
