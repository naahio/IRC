/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 10:13:49 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/13 16:31:11 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../server/server.hpp"

void	Server::JOINcmd(int fd, std::vector<std::string> &cmd)
{
	User *user;
	std::vector<std::string> channels;
	std::vector<std::string> keys;

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
				this->createChannel(channels[i], *user, keys[i]);
            send(fd,"Mar7ba bik f channel ",
                strlen("Mar7ba bik f channel "), 0);
            send(fd,channels[i].c_str(),
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
		throw myException(ERR_ALREADYREGISTRED);
	else if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);
	else
	{
		user->setPassword(cmd[1]);
		user->setConnected();
	}
}

void	Server::USERcmd(int fd, std::vector<std::string> &cmd)
{
	User *user;

	user = this->getUser(fd);
	if (!user)
		return ;
	if (user->isAuth())
		throw myException(ERR_ALREADYREGISTRED);
	else if (cmd.size() < 5)
		throw myException(ERR_NEEDMOREPARAMS);
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
			send(fd, "you have been registered\n",
				sizeof("you have been registered\n"), 0);
			user->setRegistered();
		}
		else
		{
			send(fd, "hh password ghalat\n", 
				sizeof("hh password ghalat\n"), 0);
			this->clientDisconnect(user->getFd());
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
		throw myException(ERR_NONICKNAMEGIVEN);
	else
	{
		if (!paramsChecker(cmd[1]))
			throw myException(ERR_ERRONEUSNICKNAME);
		else if (this->getUser(cmd[1]))
			throw myException(ERR_ERRONEUSNICKNAME);
		user->setNickname(cmd[1]);
	}
	if (!user->isRegistered() && user->isAuth())
	{
		if (user->isConnected() && user->getPassword() == this->getPass())
		{
			send(fd, "you have been registered\n", 
				sizeof("you have been registered\n"), 0);
			user->setRegistered();
		}
		else
		{
			send(fd, "hh u cant register\n", 
				sizeof("hh u cant register\n"), 0);
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
	// here we send RPL_INVITING reply
	
	
}

void    Server::kick(std::vector<std::string> &cmd, int fd_u)
{

    if (cmd.size() < 3)
        throw myException(ERR_NEEDMOREPARAMS);
    Channel *channel = this->getChannel(cmd[1]);
	std::string	erply = ":";
    if (!channel)
        throw myException(ERR_NOSUCHCHANNEL);
    if (!channel->getOperator(fd_u))
        throw myException(ERR_CHANOPRIVSNEEDED);
    User *user = this->getUser(cmd[2]);
    if (!user || !channel->getMember(user->getFd()))
        throw myException(ERR_NOSUCHNICK);
    channel->getMembers().erase(user->getFd());
	erply += channel->getOperator(fd_u)->getNickname();
	erply += "!~";
	erply += channel->getOperator(fd_u)->getUsername();
	erply += "@10.13.6.10 ";
	erply += cmd[0];
	erply += " ";
	erply += cmd[1];
	erply += " ";
	erply += cmd[2];
	erply += " :";
	erply += channel->getOperator(fd_u)->getNickname();
    channel->broadCastMessage(erply, fd_u);
}

void   Server::part(std::vector<std::string> &cmd, int fd_u)
{
	Channel	*channel;
	User	*member;
	std::string	erply = ":";

    if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);
	channel = this->getChannel(cmd[1]);
	if (!channel)
		throw myException(ERR_NOSUCHCHANNEL);
	member = channel->getMember(fd_u);
	if (!member)
		throw myException(ERR_NOTONCHANNEL);
	erply += member->getNickname();
	erply += "!~";
	erply += member->getUsername();
	erply += "@10.13.6.10 ";
	erply += cmd[0];
	erply += " ";
	erply += cmd[1];
	channel->broadCastMessage(erply, fd_u);
	channel->getMembers().erase(fd_u);
}

void    Server::mode(Channel &channel)
{
    (void)channel;
}