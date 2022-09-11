/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 10:13:49 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/11 09:50:48 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../server/server.hpp"

void	JOINcmd(Msg &msg,std::vector<std::string> &cmd, Server &server)
{
	User *user;
	std::vector<std::string> channels;
	std::vector<std::string> keys;

	user = server.getUser(msg.getSender());
	if (!user)
		return ;
	if (cmd.size() < 2)
	{
		send(msg.getSender(), err_reply(ERR_NEEDMOREPARAMS,"").c_str(),
			err_reply(ERR_NEEDMOREPARAMS,"").length(), 0);
		return;
	}

	split(cmd[1],',',channels);  // vector of channels;
	if (cmd.size() > 2)
		split(cmd[2],',',keys);	// vector of the keys on the input if they exist
	if (keys.size() < channels.size())		
		keys.resize(channels.size()," ");

	for (size_t i = 0 ; i < channels.size() ; i++)
	{
		try {
			Channel *chan = server.getChannel(channels[i]);
			if (chan)
				chan->addMember(user,keys[i]);
			else
				server.createChannel(channels[i], *user);
		}
		catch (myException &e) {
			send(msg.getSender(), e.what(), strlen(e.what()), 0);
			continue;
		}
		send(msg.getSender(),"Mar7ba bik f channel hh\n",
			strlen("Mar7ba bik f channel hh\n"), 0);
	}
}

void	PRIVMSGcmd(Msg &msg,std::vector<std::string> &cmd, Server &server)
{
	User *user,*target;
	Channel *chan;

	chan = server.getChannel(cmd[1]);
	target = server.getUser(cmd[1]);
	user = server.getUser(msg.getSender());
	if (!user)
		return ;
	if (cmd.size() == 1)
	{
		send(msg.getSender(), err_reply(ERR_NORECIPIENT,"PRIVMSG").c_str(),
			err_reply(ERR_NORECIPIENT,"PRIVMSG").length(), 0);
		return;
	}
	else if (cmd.size() == 2)
	{
		send(msg.getSender(), err_reply(ERR_NOTEXTTOSEND,"").c_str(),
			err_reply(ERR_NOTEXTTOSEND,"").length(), 0);
		return ;
	}
	else
	{
		if (chan)
		{	
			try {
				cmd[2] += "\n";
				chan->broadCastMessage(cmd[2],user->getFd());
			}
			catch (myException &e )
			{
				send(msg.getSender(),e.what(),strlen(e.what()),0);
			}
		}
		else if (target)
		{
			// std::cout << "im here" << std::endl;
			std::string	reply;
			cmd[2] += '\n';
			reply = ":";
			reply += user->getNickname();
			reply += " ";
			reply += "PRIVMSG ";
			reply += target->getNickname();
			reply += ":";
			reply += cmd[2].c_str();
			if (send(target->getFd(),reply.c_str(), reply.length(), 0) == -1)
				std::cout << "sending error" << std::endl;
		}
		else if (!target && !chan)
		{
			send(msg.getSender(), err_reply(ERR_NOSUCHNICK,cmd[1]).c_str(),
			err_reply(ERR_NOSUCHNICK,cmd[1]).length(), 0);
		}
	}

}

void	PASScmd(Msg &msg,std::vector<std::string> &cmd, Server &server)
{
	User *user;

	user = server.getUser(msg.getSender());
	if (user->isAuth())
	{
		send(msg.getSender(), err_reply(ERR_ALREADYREGISTRED,"").c_str(), 
					err_reply(ERR_ALREADYREGISTRED,"").length(), 0);
		return ;
	}
	else if (cmd.size() < 2)
	{
		send(msg.getSender(), err_reply(ERR_NEEDMOREPARAMS,"").c_str(), 
			err_reply(ERR_NEEDMOREPARAMS,"").length(), 0);
		return ;
	}
	else
	{
		user->setPassword(cmd[1]);
		user->setConnected();
	}
}

void	USERcmd(Msg &msg,std::vector<std::string> &cmd, Server &server)
{
	User *user;

	user = server.getUser(msg.getSender());
	if (!user)
		return ;
	if (user->isAuth())
	{
		send(msg.getSender(), err_reply(ERR_ALREADYREGISTRED,"").c_str(), 
				err_reply(ERR_ALREADYREGISTRED,"").length(), 0);
		return ;
	}
	else if (cmd.size() < 5)
	{
		send(msg.getSender(), err_reply(ERR_NEEDMOREPARAMS,"").c_str(), 
			err_reply(ERR_NEEDMOREPARAMS,"").length(), 0);
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
		if (user->isConnected() && user->getPassword() == server.getPass())
		{
			send(msg.getSender(), "you have been registered\n", 
				sizeof("you have been registered\n"), 0);
			user->setRegistered();
		}
		else
		{
			send(msg.getSender(), "hh password ghalat\n", 
				sizeof("hh password ghalat\n"), 0);
			server.clientDisconnect(user->getFd());
		}
	}
}

void	NICKcmd(Msg &msg,std::vector<std::string> &cmd, Server &server)
{
	User *user;

	user = server.getUser(msg.getSender());
	if (!user)
		return;
	if (cmd.size() < 2)
	{
		send(msg.getSender(), err_reply(ERR_NONICKNAMEGIVEN,"").c_str(), 
			err_reply(ERR_NONICKNAMEGIVEN,"").length(), 0);
		return ;
	}
	else
	{
		if (!paramsChecker(cmd[1]))
		{
			send(msg.getSender(), err_reply(ERR_ERRONEUSNICKNAME, cmd[1]).c_str(), 
				err_reply(ERR_ERRONEUSNICKNAME,cmd[1]).length(), 0);
			return ;
		}
		else if (server.getUser(cmd[1]))
		{
			send(msg.getSender(), err_reply(ERR_NICKNAMEINUSE,cmd[1]).c_str(), 
			err_reply(ERR_NICKNAMEINUSE,cmd[1]).length(), 0);

		}
		user->setNickname(cmd[1]);
	}
	if (!user->isRegistered() && user->isAuth())
	{
		if (user->isConnected() && user->getPassword() == server.getPass())
		{
			send(msg.getSender(), "you have been registered\n", 
				sizeof("you have been registered\n"), 0);
			user->setRegistered();
		}
		else
		{
			send(msg.getSender(), "hh u cant register\n", 
				sizeof("hh u cant register\n"), 0);
			server.clientDisconnect(user->getFd());
		}
	}
}

void    helps(int fd)
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
    // std::cout << "|-> <PASS> 'password' " << std::endl;
    // std::cout << "|-> <USER> 'username' 'hostname' 'servername' 'realname'" << std::endl;
    // std::cout << "|-> <NICK> 'nickname' " << std::endl;
    // std::cout << "Channel commands : " << std::endl;
    // std::cout << "|-> <JOIN>  '<#>channel name'. . . 'key'. . ." <<std::endl;
    // std::cout << "|-> <KICK>  '<#>channel name'. . . 'user'. . ." <<std::endl;
    // std::cout << "|-> <PART>  '<#>channel name'" <<std::endl;
    // std::cout << "|-> <MODE>  '<#>channel name' <mode>" <<std::endl;
    // std::cout << "|-> <USERMODES>  'mode'" <<std::endl;
    // std::cout << "|-> <USERMODES>  'mode'" <<std::endl;
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

// void    kick(int fd)
// {

// }

// void    part(int fd)
// {
    
// }