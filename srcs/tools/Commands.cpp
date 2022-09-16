/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 10:13:49 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/16 16:26:42 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../server/server.hpp"

// function for NAMES command , It helps in join too

void	sendChannelUsers(int fd, Channel *chan,User *user,const std::string & channel)
{
	std::map<int, User *>::iterator	it;
	std::map <int, User *>			chanMembers;
	std::string						members;

	chanMembers = chan->getMembers();
	for (it = chanMembers.begin();it != chanMembers.end();it++)
	{
		if (it->second->isVisible())
			members += (it->second->getNickname() + " ");
	}
	sendReply(fd, stringBuilder(6,":irc!~irc1337 353 ",
			user->getNickname().c_str()," = ",channel.c_str(),
			" : ", members.c_str()));
}

void	Server::JOINcmd(int fd, std::vector<std::string> &cmd)
{
	User						*user;
	std::vector<std::string>	channels;
	std::vector<std::string>	keys;
	std::string					reply;

	user = this->getUser(fd);
	if (!user)
		return ;
	if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);
    // join chan1 " "
	split(cmd[1],',',channels);  // vector of channels;
	if (cmd.size() > 2)
		split(cmd[2],',',keys);	// vector of the keys on the input if they exist
	if (keys.size() < channels.size())		
		keys.resize(channels.size(),"");

	for (size_t i = 0 ; i < channels.size() ; i++)
	{
		try {
			Channel *chan = this->getChannel(channels[i]);
			if (chan)
				chan->addMember(user, keys[i]);
			else
				this->createChannel(channels[i], *user);
           	reply = stringBuilder(8, ":",user->getNickname().c_str(), "!~", user->getUsername().c_str(),
				"@10.13.6.10 ", cmd[0].c_str(), " :", channels[i].c_str());
			std::cout << reply << std::endl;
			this->getChannel(channels[i])->broadCastMessage(reply);
			sendChannelUsers(fd, this->getChannel(channels[i]), user, 
					this->getChannel(channels[i])->getName());
			sendReply(fd, stringBuilder(5, ":irc!~irc1337 366 ",
						user->getNickname().c_str(), " ", 
						this->getChannel(channels[i])->getName().c_str(),
						" :End of /NAMES list."));
			reply.clear();
		}
		catch (myException &e) {
			send(fd, e.what(), strlen(e.what()), 0);
		}
	}
}
void	Server::PRIVMSGcmd(int fd, std::vector<std::string> &cmd)
{
	User *user,*target;
	Channel *chan;

	chan = this->getChannel(cmd[1]);
	target = this->getUser(cmd[1]);
	user = this->getUser(fd);
	if (!user)
		return ;
	if (cmd.size() == 1)
		throw myException(ERR_NORECIPIENT);
	else if (cmd.size() == 2)
		throw myException(ERR_NOTEXTTOSEND);
	else
	{
		if (chan)
		{	
			try {
				cmd[2] += "\n";
				std::string reply;
				reply = ":";
				reply += user->getNickname();
				reply += "!~";
				reply += user->getUsername();
				reply += "@10.13.6.10 ";
				reply += "PRIVMSG ";
				reply += chan->getName();
				reply += " :";
				reply += cmd[2].c_str();
				chan->broadCastMessage(reply, user->getFd());
			}
			catch (myException &e )
			{
				send(fd,e.what(),strlen(e.what()),0);
			}
		}
		else if (target)
		{
			std::string	reply;
			cmd[2] += '\n';
			reply = stringBuilder(9, ":", user->getNickname().c_str(), "!~", user->getUsername().c_str(),
					"@10.13.6.10 ", "PRIVMSG ", target->getNickname().c_str(), " :", cmd[2].c_str());
			if (send(target->getFd(),reply.c_str(), reply.length(), 0) == -1)
				std::cout << "sending error" << std::endl;
		}
		else if (!target && !chan)
		{
			send(fd, err_reply(ERR_NOSUCHNICK).c_str(),
			err_reply(ERR_NOSUCHNICK).length(), 0);
		}
	}

}

void	Server::PASScmd(int fd, std::vector<std::string> &cmd)
{
	User *user;

	user = this->getUser(fd);
	if (user->isAuth())
	{
		sendReply(fd,stringBuilder(4,":irc!~irc1337 ",ft_tostring(ERR_ALREADYREGISTRED).c_str(),
			" PASS ",err_reply(ERR_ALREADYREGISTRED).c_str()));
		return ;
	}
	else if (cmd.size() < 2)
	{
		sendReply(fd,stringBuilder(4,":irc!~irc1337 ",ft_tostring(ERR_NEEDMOREPARAMS).c_str(),
			" PASS ",err_reply(ERR_NEEDMOREPARAMS).c_str()));
		return ;
	}
	else
	{
		user->setPassword(cmd[1]);
		user->setConnected();
	}
}

void	Server::USERcmd(int fd, std::vector<std::string> &cmd)
{
	User *user;
	std::string reply;
	user = this->getUser(fd);
	if (!user)
		return ;
	if (user->isAuth())
	{
		sendReply(fd,stringBuilder(4,":irc!~irc1337 ",ft_tostring(ERR_ALREADYREGISTRED).c_str(),
			" USER ",err_reply(ERR_ALREADYREGISTRED).c_str()));
		return ;
	}
	else if (cmd.size() < 5)
	{
		sendReply(fd,stringBuilder(4,":irc!~irc1337 ",ft_tostring(ERR_NEEDMOREPARAMS).c_str(),
			" USER ",err_reply(ERR_NEEDMOREPARAMS).c_str()));
		return ;
	}
	else
	{
		user->setUsername(cmd[1]);
		user->setHostName(cmd[2]);
		user->setServerName(cmd[3]);
		user->setFullName(cmd[4]);
	}
	if (user->isAuth())
	{
		if (user->isConnected() && user->getPassword() == this->getPass())
		{
			sendReply(fd, stringBuilder(8,":irc!~irc1337 001 ",
					user->getNickname().c_str(),
					" :Welcome to the Internet Relay Network ",
					user->getNickname().c_str(),"!",
					user->getUsername().c_str(),
					"@",user->getIpAddress().c_str()));
			user->setRegistered();
		}
		else
		{
			sendReply(fd,stringBuilder(4,":irc!~irc1337 ",ft_tostring(ERR_PASSWDMISMATCH).c_str(),
			" PASS ",err_reply(ERR_PASSWDMISMATCH).c_str()));
		}
	}
}

void	Server::NICKcmd(int fd, std::vector<std::string> &cmd)
{
	User *user;

	user = this->getUser(fd);
	if (!user)
		return;
	if (cmd.size() < 2)
	{
		sendReply(fd,stringBuilder(4,":irc!~irc1337 ",ft_tostring(ERR_NONICKNAMEGIVEN).c_str(),
			" NICK :",err_reply(ERR_NONICKNAMEGIVEN).c_str()));
		return ;
	}
	else
	{
		if (!paramsChecker(cmd[1]))
		{
			sendReply(fd,stringBuilder(6,":irc!~irc1337 ",ft_tostring(ERR_ERRONEUSNICKNAME).c_str(),
			" ",cmd[1].c_str()," ",err_reply(ERR_ERRONEUSNICKNAME).c_str()));
			return ;
		}
		else if (this->getUser(cmd[1]))
		{
			sendReply(fd,stringBuilder(6,":irc!~irc1337 ",ft_tostring(ERR_NICKNAMEINUSE).c_str(),
			" ",cmd[1].c_str()," ",err_reply(ERR_NICKNAMEINUSE).c_str()));
			return ;
		}
		user->setNickname(cmd[1]);
	}
	if (!user->isRegistered() && user->isAuth())
	{
		if (user->isConnected() && user->getPassword() == this->getPass())
		{
			sendReply(fd, stringBuilder(8,":irc!~irc1337 001 ",
					user->getNickname().c_str(),
					" :Welcome to the Internet Relay Network ",
					user->getNickname().c_str(),"!",
					user->getUsername().c_str(),
					"@",user->getIpAddress().c_str()));
			user->setRegistered();
		}
		else
		{
			sendReply(fd,stringBuilder(4,":irc!~irc1337 ",
			ft_tostring(ERR_PASSWDMISMATCH).c_str(),
			" PASS ",err_reply(ERR_PASSWDMISMATCH).c_str()));
			this->clientDisconnect(user->getFd());
		}
	}
}

void    Server::helps(int fd)
{
    send(fd, ":IRC_1337 NOTICE HELP :use te following commands to register or log in : \n", sizeof(":IRC_1337 NOTICE HELP :use te following commands to register or log in : \n"), 0);
    send(fd, "|-> <PASS> 'password' \n", sizeof("|-> <PASS> 'password' \n"), 0);
    send(fd, "|-> <USER> 'username' 'hostname' 'servername' 'realname' \n", sizeof("|-> <USER> 'username' 'hostname' 'servername' 'realname' \n"), 0);
    send(fd, "|-> <NICK> 'nickname' \n", sizeof("|-> <NICK> 'nickname' \n"), 0);
    send(fd, "Channel commands : \n", sizeof("Channel commands : \n"), 0);
    send(fd, "|-> <JOIN>  '<#>channel name'. . . 'key'. . . \n", sizeof("|-> <JOIN>  '<#>channel name'. . . 'key'. . . \n"), 0);
    send(fd, "|-> <KICK>  '<#>channel name'. . . 'user'. . . \n", sizeof("|-> <KICK>  '<#>channel name'. . . 'user'. . . \n"), 0);
    send(fd, "|-> <PART>  '<#>channel name' \n", sizeof("|-> <PART>  '<#>channel name' \n"), 0);
    send(fd, "|-> <MODE>  '<#>channel name' <mode> \n", sizeof("|-> <MODE>  '<#>channel name' <mode> \n"), 0);
    send(fd, "|-> <USERMODES>  'mode' \n", sizeof("|-> <USERMODES>  'mode' \n"), 0);
    send(fd, "|-> <LIST>  '<#>channel name' 'server' \n", sizeof("|-> <LIST>  '<#>channel name' 'server' \n"), 0);
    send(fd, "|-> <INVITE>  'nickname' '<#>channel name' \n", sizeof("|-> <INVITE>  'nickname' '<#>channel name' \n"), 0);
    send(fd, "Server commands : \n", sizeof("Server commands : \n"), 0);
    send(fd, "|=> <VERSION> \n", sizeof("|=> <VERSION> \n"), 0);
    send(fd, "|=> <TIME>  \n", sizeof("|=> <TIME>  \n"), 0);
    send(fd, "|=> <ADMIN> \n", sizeof("|=> <ADMINE> \n"), 0);
    send(fd, "|=> <INFO>  \n", sizeof("|=> <INFO>  \n"), 0);
    send(fd, "|=> <KILL> 'nickname' 'reason' \n", sizeof("|=> <KILL> 'nickname' 'reason' \n"), 0);
    send(fd, "Cient to client / channel commandes : \n", sizeof("Cient to client / channel commandes : \n"), 0);
    send(fd, "|-> <PRIVMSG> 'receiver' 'message' \n", sizeof("|-> <PRIVMSG> 'receiver' 'message' \n"), 0);
	// add QUIT 
}

void	Server::INVITcmd(int fd,std::vector<std::string> &cmd)
{
	User *user;
	User *invit;
	Channel *channel;

	user = this->getUser(fd);
	if (!user)
		return;
	if (cmd.size() < 3)
		throw myException(ERR_NEEDMOREPARAMS);
	channel = this->getChannel(cmd[2]);
	if (!channel)
		throw myException(ERR_NOSUCHCHANNEL);
	if (!channel->getMember(fd))
		throw myException(ERR_NOTONCHANNEL);
	if (channel->isInviteOnly() && !channel->getOperator(user->getFd()))
		throw myException(ERR_CHANOPRIVSNEEDED);
	invit = this->getUser(cmd[1]);
	if (!invit)
		throw myException(ERR_NOSUCHNICK);
	if (channel->getMember(invit->getFd()))
		throw myException(ERR_USERONCHANNEL);
	channel->addInvitee(invit->getFd());
	send(fd,"hh sardtlo invite\n",sizeof("hh sardtlo invite\n"),0);
	// here we send RPL_INVITING reply

}

void    Server::kick(int fd, std::vector<std::string> &cmd)
{
	User *user;

    if (cmd.size() < 3)
        throw myException(ERR_NEEDMOREPARAMS);
    Channel *channel = this->getChannel(cmd[1]);
	std::string	erply = ":";
    if (!channel)
        throw myException(ERR_NOSUCHCHANNEL);
	user = channel->getOperator(fd);
    if (!user)
        throw myException(ERR_CHANOPRIVSNEEDED);
    user = this->getUser(cmd[2]);
    if (!user || !channel->getMember(user->getFd()))
        throw myException(ERR_NOSUCHNICK);
	erply = stringBuilder(9, ":",user->getNickname().c_str(), "!~", user->getUsername().c_str(),
				"@10.13.6.10 ", cmd[0].c_str(), " ", channel->getName().c_str(), "\n");
	channel->getMembers().erase(user->getFd());
    channel->broadCastMessage(erply, fd);
}

void   Server::part(int fd, std::vector<std::string> &cmd)
{
	Channel						*channel;
	User						*member;
	std::string					reply ;
	std::vector<std::string>	chans;
	int							i = 0;

    if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);
	split(cmd[1], ',', chans);
	while (i < chans.size())
	{
		channel = this->getChannel(chans[i]);
		if (!channel)
			throw myException(ERR_NOSUCHCHANNEL);
		member = channel->getMember(fd);
		if (!member)
			throw myException(ERR_NOTONCHANNEL);
		reply = stringBuilder(9, ":",member->getNickname().c_str(), "!~", member->getUsername().c_str(),
				"@10.13.6.10 ", cmd[0].c_str(), " ", chans[i].c_str(), "\n");
		channel->broadCastMessage(reply, fd);
		channel->getMembers().erase(fd);
		i++;
	}
}

void	Server::list(int fd, std::vector<std::string> &cmd)
{
	std::string					reply;
	std::vector<std::string>	channel;
	int							i = 0;
	Channel						*chans;

	reply = stringBuilder(2, ":irc!~irc1337@10.13.6.10 321 ", " :Channel :Users  Name");
	sendReply(fd, reply);
	if (cmd.size() == 1)
	{
		std::map <std::string, Channel *>::iterator	it;

		for (it = this->channels.begin(); it != this->channels.end(); ++it)
		{
			if (!it->second->isPrivate())
				reply = stringBuilder(8, ":irc!~irc1337@10.13.6.10 322 ", this->getUser(fd)->getNickname().c_str(), " ", it->first.c_str(), " ",ft_tostring(it->second->getMembers().size()).c_str(), " :", it->second->getTopic().c_str());
			else
				reply = stringBuilder(7, ":irc!~irc1337@10.13.6.10 322 ", this->getUser(fd)->getNickname().c_str(), " ", it->first.c_str(), " ",ft_tostring(it->second->getMembers().size()).c_str(), " :");
			sendReply(fd, reply);
		}
		split(cmd[1], ',', channel);
		while (i < channel.size())
		{	
			chans = this->getChannel(channel[i]);
			if (chans)
			{
				if (!chans->isPrivate())
					reply = stringBuilder(8, ":irc!~irc1337@10.13.6.10 322 ", this->getUser(fd)->getNickname().c_str(), " ", chans->getName().c_str(), " ",ft_tostring(chans->getMembers().size()).c_str(), " :", chans->getTopic().c_str());
				else
					reply = stringBuilder(7, ":irc!~irc1337@10.13.6.10 322 ", this->getUser(fd)->getNickname().c_str(), " ", chans->getName().c_str(), " ",ft_tostring(chans->getMembers().size()).c_str(), " :");
				sendReply(fd, reply);
			}
			i++;
		}
	}
	reply = stringBuilder(3, ":irc!~irc1337@10.13.6.10 323 ", this->getUser(fd)->getNickname().c_str(), " :End of /LIST");
	sendReply(fd, reply);
	sendReply(fd, reply);
}

void	Server::channelModes(int fd, std::vector<std::string> & cmd) {
	Channel *			channel;
	User *				user;
	std::stringstream	ss;
	int					sign = 1;
	int					argId = 3;

	channel = this->getChannel(cmd[1]);
	if (!channel)
		throw myException(ERR_NOSUCHCHANNEL);
	if (!channel->getOperator(fd))
		throw myException(ERR_CHANOPRIVSNEEDED);
	for (int i = 0; i < cmd[2].length(); i++) {
		try {
			switch (cmd[2][i]) {
				case '+':
					sign = 1;
					break;
				case '-':
					sign = 0;
					break;
				case 'o':
					if (cmd.size() < 3)
						break;
					user = this->getUser(cmd[argId++]);
					if (!user)
						throw myException(ERR_NOSUCHNICK);
					if (sign == 1) {
						channel->addOperator(user->getFd());
					} else if (sign == 0) {
						channel->removeOperator(user->getFd());
					}
					break;
				case 'p':
					if (sign == 1) {
						channel->setPrivate(true, fd);
					} else if (sign == 0) {
						channel->setPrivate(false, fd);
					}
					break;
				case 's':
					if (sign == 1) {
						channel->setSecret(true, fd);
					} else if (sign == 0) {
						channel->setSecret(false, fd);
					}
					break;
				case 'i':
					if (sign == 1) {
						channel->setInviteOnly(true, fd);
					} else if (sign == 0) {
						channel->setInviteOnly(false, fd);
					}
					break;
				case 't':
					if (sign == 1) {
						channel->setTopicSettable(true, fd);
					} else if (sign == 0) {
						channel->setTopicSettable(false, fd);
					}
					break;
				case 'n':
					if (sign == 1) {
						channel->setMemberChatOnly(true, fd);
					} else if (sign == 0) {
						channel->setMemberChatOnly(false, fd);
					}
					break;
				case 'm':
					if (sign == 1) {
						channel->setModerated(true, fd);
					} else if (sign == 0) {
						channel->setModerated(false, fd);
					}
					break;
				case 'l':
					if (cmd.size() < 3)
						throw myException(ERR_NEEDMOREPARAMS);
					if (sign == 1) {
						int	limit;
						ss << cmd[argId++];
						ss >> limit;
						channel->setLimit(limit, fd);
					} else if (sign == 0) {
						channel->setLimit(-1, fd);
					}
					break;
				// case 'b':
					// ban masks
				case 'v':
					if (cmd.size() < 3)
						break;
					user = this->getUser(cmd[argId++]);
					if (!user)
						throw myException(ERR_NOSUCHNICK);
					if (sign == 1) {
						channel->addModerator(user->getFd());
					} else if (sign == 0) {
						channel->removeModerator(user->getFd());
					}
					break;
				case 'k':
					if (cmd.size() < 3)
						break;
					if (sign == 1) {
						channel->setKey(cmd[argId++], fd);
					} else if (sign == 0) {
						channel->setKey("", fd);
					}
					break;
				default:
					throw myException(ERR_UNKNOWNMODE);
			}
		} catch (std::exception & e) {
			send(fd, e.what(), strlen(e.what()), 0);
		}
	}
}

void	Server::userModes(int fd, std::vector<std::string> & cmd) {
	User *				user;
	std::stringstream	ss;
	int					sign = 1;
	int					argId = 3;

	user = this->getUser(cmd[1]);
	if (!user)
		throw myException(ERR_NOSUCHCHANNEL);
	if (user->getFd() != fd)
		throw myException(ERR_USERSDONTMATCH);
	for (int i = 0; i < cmd[2].length(); i++) {
		try {
			switch (cmd[2][i]) {
				case '+':
					sign = 1;
					break;
				case '-':
					sign = 0;
					break;
				case 'i':
					if (sign == 1) {
						user->setVisibility(true);
					} else if (sign == 0) {
						user->setVisibility(false);
					}
					break;
				default:
					throw myException(ERR_UMODEUNKNOWNFLAG);
			}
		} catch (std::exception & e) {
			send(fd, e.what(), strlen(e.what()), 0);
		}
	}
}

void	Server::mode(int fd, std::vector<std::string> & cmd) {
	try {
		if (cmd.size() < 2)
			throw myException(ERR_NEEDMOREPARAMS);
		if (cmd[1][0] == '#') {
			channelModes(fd, cmd);
		} else {
			userModes(fd, cmd);
		}
	} catch (std::exception & e) {
		throw myException(std::string(e.what()));
	}
}


void	Server::names(int fd_u, std::vector<std::string> &cmd)
{
	std::string						reply;
	std::vector<std::string>		channel;
	std::map <int, User *>			chanMembers;
	User							*user;
	Channel							*chan;

	user = this->getUser(fd_u);
	if (!user)
		return ;
	if (cmd.size() > 1)
		split(cmd[1],',',channel);
	if (channel.size() > 0)
	{
		for (size_t i = 0; i < channel.size(); i++)
		{
			chan = this->getChannel(channel[i]);
			if (chan && !chan->isPrivate() && !chan->isSecret())
			{
				sendChannelUsers(fd_u, chan, user,channel[i]);

				sendReply(fd_u, stringBuilder(5, ":irc!~irc1337 366 ",
						user->getNickname().c_str(), " ", channel[i].c_str(),
						" :End of /NAMES list."));
			}
			else
				sendReply(fd_u, stringBuilder(5, ":irc!~irc1337 366 ",
					user->getNickname().c_str(), " ", channel[i].c_str(),
					" :End of /NAMES list."));
		}
	}
	else
	{
		std::map <std::string, Channel *>::iterator	  it ;
		std::map <std::string, Channel *> serverChannels =  this->getChannels();
		
		for (it = serverChannels.begin(); it != serverChannels.end(); it++)
		{
			if (!it->second->isPrivate() && !it->second->isSecret())
				sendChannelUsers(fd_u, it->second, user,it->second->getName());
		}
		sendReply(fd_u, stringBuilder(4, ":irc!~irc1337 366 ",
					user->getNickname().c_str(), " * ",
					" :End of /NAMES list."));
	}
}


void    Server::QUITcmd(int fd, std::vector<std::string> & cmd)
{
	std::string reply;
	User *user;

	user = this->getUser(fd);
	if (!user)
		return ;
	if (!cmd[1].empty())
		reply = stringBuilder(5, "ERROR :Closing Link: ", user->getIpAddress().c_str(), " (Quit: ", cmd[1].c_str(), ")");
	else
		reply = stringBuilder(5, "ERROR :Closing Link: ", user->getIpAddress().c_str(), " (Quit: ", user->getNickname().c_str(), ")");
	sendReply(fd,reply);
	this->clientDisconnect(fd);
}

void	Server::OPERcmd(int fd, std::vector<std::string> &cmd)
{
	User *user;
	user = this->getUser(fd);
	if (!user)
		return;
	if (cmd.size() < 3)
		throw myException(ERR_NEEDMOREPARAMS);
	std::map <std::string, std::string>::iterator it;
	it = this->getOperators().find(cmd[1]);
	if (it == this->getOperators().end())
		return ;
	else
	{
		if (it->second == cmd[2])
		{
			user->setIsOperator();
			sendReply(fd, stringBuilder(5,":irc!~irc1337 381 ",
						user->getNickname().c_str()," :You are now an IRC operator"));
		}
		else
			throw myException(ERR_PASSWDMISMATCH);
	}
	
}