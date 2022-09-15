/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 10:13:49 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/15 13:53:09 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../server/server.hpp"

void	Server::JOINcmd(int fd, std::vector<std::string> &cmd)
{
	User *user;
	std::vector<std::string> channels;
	std::vector<std::string> keys;
	std::string replay;

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
			replay = stringBuilder(7,user->getNickname().c_str(),"!~",
			user->getUsername().c_str(),
                channels[i].length(), 0);
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
				// reply = stringBuilder(9, ":", user->getNickname(), );
				chan->broadCastMessage(reply, user->getFd());
			}
			catch (myException &e )
			{
				send(fd,e.what(),strlen(e.what()),0);
			}
		}
		else if (target)
		{
			// std::cout << "im here" << std::endl;
			std::string	reply;
			cmd[2] += '\n';
			reply = ":";
			reply += user->getNickname();
			reply += "!~";
			reply += user->getUsername();
			reply += "@10.13.6.10 ";
			reply += "PRIVMSG ";
			reply += target->getNickname();
			reply += " :";
			reply += cmd[2].c_str();
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
		sendReplay(fd,stringBuilder(4,":irc!~irc1337 ",ft_tostring(ERR_ALREADYREGISTRED).c_str(),
			" PASS ",err_reply(ERR_ALREADYREGISTRED).c_str()));
		return ;
	}
	else if (cmd.size() < 2)
	{
		sendReplay(fd,stringBuilder(4,":irc!~irc1337 ",ft_tostring(ERR_NEEDMOREPARAMS).c_str(),
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
		sendReplay(fd,stringBuilder(4,":irc!~irc1337 ",ft_tostring(ERR_ALREADYREGISTRED).c_str(),
			" USER ",err_reply(ERR_ALREADYREGISTRED).c_str()));
		return ;
	}
	else if (cmd.size() < 5)
	{
		sendReplay(fd,stringBuilder(4,":irc!~irc1337 ",ft_tostring(ERR_NEEDMOREPARAMS).c_str(),
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
			sendReplay(fd, stringBuilder(8,":irc!~irc1337 001 ",
					user->getNickname().c_str(),
					" :Welcome to the Internet Relay Network ",
					user->getNickname().c_str(),"!",
					user->getUsername().c_str(),
					"@",user->getIpAddress().c_str()));
			user->setRegistered();
		}
		else
		{
			sendReplay(fd,stringBuilder(4,":irc!~irc1337 ",ft_tostring(ERR_PASSWDMISMATCH).c_str(),
			" PASS :",err_reply(ERR_PASSWDMISMATCH).c_str()));
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
		sendReplay(fd,stringBuilder(4,":irc!~irc1337 ",ft_tostring(ERR_NONICKNAMEGIVEN).c_str(),
			" NICK :",err_reply(ERR_NONICKNAMEGIVEN).c_str()));
		return ;
	}
	else
	{
		if (!paramsChecker(cmd[1]))
		{
			sendReplay(fd,stringBuilder(6,":irc!~irc1337 ",ft_tostring(ERR_ERRONEUSNICKNAME).c_str(),
			" ",cmd[1].c_str()," ",err_reply(ERR_ERRONEUSNICKNAME).c_str()));
			return ;
		}
		else if (this->getUser(cmd[1]))
		{
			sendReplay(fd,stringBuilder(6,":irc!~irc1337 ",ft_tostring(ERR_NICKNAMEINUSE).c_str(),
			" ",cmd[1].c_str()," ",err_reply(ERR_NICKNAMEINUSE).c_str()));
			return ;
		}
		user->setNickname(cmd[1]);
	}
	if (!user->isRegistered() && user->isAuth())
	{
		if (user->isConnected() && user->getPassword() == this->getPass())
		{
			sendReplay(fd, stringBuilder(8,":irc!~irc1337 001 ",
					user->getNickname().c_str(),
					" :Welcome to the Internet Relay Network ",
					user->getNickname().c_str(),"!",
					user->getUsername().c_str(),
					"@",user->getIpAddress().c_str()));
			user->setRegistered();
		}
		else
		{
			sendReplay(fd,stringBuilder(4,":irc!~irc1337 ",
			ft_tostring(ERR_PASSWDMISMATCH).c_str(),
			" PASS :",err_reply(ERR_PASSWDMISMATCH).c_str()));
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
    send(fd, "|=> <ADMINE> \n", sizeof("|=> <ADMINE> \n"), 0);
    send(fd, "|=> <INFO>  \n", sizeof("|=> <INFO>  \n"), 0);
    send(fd, "|=> <KILL> 'nickname' 'reason' \n", sizeof("|=> <KILL> 'nickname' 'reason' \n"), 0);
    send(fd, "Cient to client / channel commandes : \n", sizeof("Cient to client / channel commandes : \n"), 0);
    send(fd, "|-> <PRIVMSG> 'receiver' 'message' \n", sizeof("|-> <PRIVMSG> 'receiver' 'message' \n"), 0);


    // std::cout << "use te following commands to register or log in : " << std::endl;  
    // std::cout << "|-> <PASS> 'password' " << std::endl;                                       *
    // std::cout << "|-> <USER> 'username' 'hostname' 'servername' 'realname'" << std::endl;     *
    // std::cout << "|-> <NICK> 'nickname' " << std::endl;                                       *
    // std::cout << "Channel commands : " << std::endl;
    // std::cout << "|-> <JOIN>  '<#>channel name'. . . 'key'. . ." <<std::endl;                 *
    // std::cout << "|-> <KICK>  '<#>channel name'. . . 'user'. . ." <<std::endl;                *
    // std::cout << "|-> <PART>  '<#>channel name'" <<std::endl;                                 *
    // std::cout << "|-> <MODE>  '<#>channel name' <mode>" <<std::endl;                          *
    // std::cout << "|-> <USERMODES>  'mode'" <<std::endl;                                       *
    // std::cout << "|-> <LIST>  '<#>channel name' 'server'" <<std::endl;
    // std::cout << "|-> <INVITE>  'nickname' '<#>channel name'" <<std::endl;
    // std::cout << "Server commands : " << std::endl;
    // std::cout << "|=> <VERSION> " << std::endl;
    // std::cout << "|=> <TIME> " << std::endl;
    // std::cout << "|=> <ADMINE> " << std::endl;
    // std::cout << "|=> <INFO> " << std::endl;
    // std::cout << "|=> <KILL> 'nickname' 'reason' " << std::endl;
    // std::cout << "Cient to client / channel commandes : " << std::endl;
    // std::cout << "|-> <PRIVMSG> 'receiver' 'message'" <<std::endl;

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
}

void    Server::kick(int fd, std::vector<std::string> &cmd)
{

    if (cmd.size() < 3)
        throw myException(ERR_NEEDMOREPARAMS);
    Channel *channel = this->getChannel(cmd[1]);
	std::string	erply = ":";
    if (!channel)
        throw myException(ERR_NOSUCHCHANNEL);
    if (!channel->getOperator(fd))
        throw myException(ERR_CHANOPRIVSNEEDED);
    User *user = this->getUser(cmd[2]);
    if (!user || !channel->getMember(user->getFd()))
        throw myException(ERR_NOSUCHNICK);
    channel->getMembers().erase(user->getFd());
	erply += channel->getOperator(fd)->getNickname();
	erply += "!~";
	erply += channel->getOperator(fd)->getUsername();
	erply += "@10.13.6.10 ";
	erply += cmd[0];
	erply += " ";
	erply += cmd[1];
	erply += " ";
	erply += cmd[2];
	erply += " :";
	erply += channel->getOperator(fd)->getNickname();
    channel->broadCastMessage(erply, fd);
}

void   Server::part(int fd, std::vector<std::string> &cmd)
{
	Channel	*channel;
	User	*member;
	std::string	erply = ":";

    if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);
	channel = this->getChannel(cmd[1]);
	if (!channel)
		throw myException(ERR_NOSUCHCHANNEL);
	member = channel->getMember(fd);
	if (!member)
		throw myException(ERR_NOTONCHANNEL);
	erply += member->getNickname();
	erply += "!~";
	erply += member->getUsername();
	erply += "@10.13.6.10 ";
	erply += cmd[0];
	erply += " ";
	erply += cmd[1];
	channel->broadCastMessage(erply, fd);
	channel->getMembers().erase(fd);
}

void	Server::mode(int fd, std::vector<std::string> & cmd) {
	try {
		Channel *			channel;
		User *				user;
		int					sign = 1;
		int					argId = 3;
		std::stringstream	ss;

		if (cmd.size() < 2)
			throw myException(ERR_NEEDMOREPARAMS);
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
						ss << cmd[argId++];
						int	limit;
						ss >> limit;
						if (sign == 1) {
							channel->setLimit(limit, fd);
						} else if (sign == 0) {
							channel->setLimit(-1, fd);
						}
						break;
					case 'b':
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
	} catch (std::exception & e) {
		throw myException(std::string(e.what()));
	}
}
