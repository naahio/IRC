/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 10:13:49 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/26 13:19:07 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../server/server.hpp"

// function for NAMES command , It helps in join too

void	Server::sendChannelUsers(int fd, Channel *chan,User *user,const std::string & channel)
{
	std::map<int, User *>::iterator	it;
	std::map <int, User *>			chanMembers;
	std::string						members;

	chanMembers = chan->getMembers();
	for (it = chanMembers.begin();it != chanMembers.end();it++)
	{
		if (it->second->isVisible())
		{
			if (chan->getOperator(it->second->getFd()))
				members += '@';
			members += (it->second->getNickname() + " ");
		}
	}
	sendReply(fd, this->getName()
		+ ft_tostring(RPL_NAMREPLY) + " "
		+ user->getNickname() + " = "
		+ channel + " :"
		+ members + "\n");
}

void	Server::JOINcmd(int fd, std::vector<std::string> &cmd)
{
	User						*user;
	std::vector<std::string>	channels;
	std::vector<std::string>	keys;
	std::string					rply;

	Player	*pl = this->getPlayer(fd);

	user = this->getUser(fd);
	if (!user)
		return ;
	if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);
	split(cmd[1],',',channels);  // vector of channels;
	if (cmd.size() > 2)
		split(cmd[2],',',keys);	// vector of the keys on the input if they exist
	if (keys.size() < channels.size())		
		keys.resize(channels.size(),"");

	for (size_t i = 0 ; i < channels.size() ; i++)
	{
		try {
			Channel *chan = this->getChannel(channels[i]);
			if (chan && chan->getMember(fd))
				continue;
			if (chan) {
				pl->add_Points(CHANNEL_POINT);
				chan->addMember(user, keys[i]);
			}
			else {
				pl->add_Points(CREATED_CHANNEL_POINT);
				this->createChannel(channels[i], *user);
				chan = this->getChannel(channels[i]);
			}
			rply = ":" + user->getIdentifier() + " "
				+ cmd[0] + " :"
				+ chan->getName() + "\n";
			chan->broadCastMessage(rply);
			sendChannelUsers(fd, chan, user, chan->getName());
			sendReply(fd, this->getName()
				+ ft_tostring(RPL_ENDOFNAMES) + " "
				+ user->getNickname() + " "
				+ chan->getName() + " "
				+ reply(RPL_ENDOFNAMES) + "\n");
		}
		catch (myException &e) {
			sendReply(fd, this->getName()
				+ ft_tostring(e.getERROR_NO()) + " "
				+ user->getNickname() + " "
				+ channels[i] + " "
				+ e.what() + "\n");
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
			std::string reply;
			reply = stringBuilder(10, ":", user->getNickname().c_str(), "!~", user->getUsername().c_str(), "@",user->getIpAddress().c_str(), " PRIVMSG ", chan->getName().c_str(), " :", cmd[2].c_str()), 
			chan->broadCastMessage(reply, user->getFd(), false);
		}
		else if (target)
		{
			std::vector<std::string> vec;
			if (ctcpMessage(cmd[2],vec))
				fileTransfer(fd,cmd[1],vec);
			std::string	reply;
			reply = stringBuilder(10, ":", user->getNickname().c_str(), "!~", user->getUsername().c_str(),
					"@",user->getIpAddress().c_str(), " PRIVMSG ", target->getNickname().c_str(), " :", cmd[2].c_str());
			sendReply(target->getFd(),reply);
		}
		else if (!target && !chan)
			throw myException(ERR_NOSUCHNICK);
	}
}

void	Server::PASScmd(int fd, std::vector<std::string> &cmd)
{
	User *user;

	user = this->getGuest(fd);
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
	std::string reply;
	user = this->getGuest(fd);
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
			user->setRegistered();
			this->addUser(fd,user);
			this->guests.erase(this->guests.find(fd));
			welcomeReplay(fd);
		}
		else
		{
			sendReply(fd,stringBuilder(5,this->getName().c_str()," ",ft_tostring(ERR_PASSWDMISMATCH).c_str(),
			" PASS ",err_reply(ERR_PASSWDMISMATCH).c_str()));
			cmd[1] = "BAD PASSWORD";
			QUITcmd(fd,cmd);
		}
	}
}

void	Server::NICKcmd(int fd, std::vector<std::string> &cmd)
{
	User *user;

	user = this->getGuest(fd);
	if (!user)
		return;
	if (cmd.size() < 2)
		throw myException(ERR_NONICKNAMEGIVEN);
	else
	{
		if (!paramsChecker(cmd[1]))
			throw myException(ERR_ERRONEUSNICKNAME);
		else if (this->getUser(cmd[1]) || this->getGuest(cmd[1]))
			throw myException(ERR_NICKNAMEINUSE);
		if (user->isAuth())
			sendReply(fd,stringBuilder(7,":",user->getNickname().c_str(),"!~",this->getName().c_str(),
											cmd[0].c_str()," :",cmd[1].c_str()));
		user->setNickname(cmd[1]);
	}
	if (!user->isRegistered() && user->isAuth())
	{
		if (user->isConnected() && user->getPassword() == this->getPass())
		{
			user->setRegistered();
			this->addUser(fd,user);
			this->guests.erase(this->guests.find(fd));
			welcomeReplay(fd);
		}
		else
		{
			sendReply(fd,stringBuilder(5,this->getName().c_str()," ",
			ft_tostring(ERR_PASSWDMISMATCH).c_str(),
			" PASS ",err_reply(ERR_PASSWDMISMATCH).c_str()));
			cmd[1] = "BAD PASSWORD";
			QUITcmd(fd,cmd);
		}
	}
}

void	Server::helps(int fd)
{
	sendReply(fd, this->name + "NOTICE HELP :use te following commands to register or log in : \n");
	sendReply(fd, this->name + "NOTICE HELP :|-> PASS password \n");
	sendReply(fd, this->name + "NOTICE HELP :|-> NICK nickname \n");
	sendReply(fd, this->name + "NOTICE HELP :|-> USER username hostname ervername realname \n");
	sendReply(fd, this->name + "NOTICE HELP :Channel commands : \n");
	sendReply(fd, this->name + "NOTICE HELP :|-> PRIVMSG receiver :message \n");
	sendReply(fd, this->name + "NOTICE HELP :|-> JOIN  #channel1,channel2... key1,key2. . . \n");
	sendReply(fd, this->name + "NOTICE HELP :|-> KICK  #channel user \n");
	sendReply(fd, this->name + "NOTICE HELP :|-> PART  #channel \n");
	sendReply(fd, this->name + "NOTICE HELP :|-> MODE  #channel mode \n");
	sendReply(fd, this->name + "NOTICE HELP :|-> USERMODES  mode \n");
	sendReply(fd, this->name + "NOTICE HELP :|-> LIST  null/#channel \n");
	sendReply(fd, this->name + "NOTICE HELP :|-> topic  #channel :TOPIC\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> INVITE  nickname #channel \n");
	sendReply(fd, this->name + "NOTICE HELP :Server commands : \n");
	sendReply(fd, this->name + "NOTICE HELP :|=> VERSION \n");
	sendReply(fd, this->name + "NOTICE HELP :|=> TIME  \n");
	sendReply(fd, this->name + "NOTICE HELP :|=> ADMIN \n");
	sendReply(fd, this->name + "NOTICE HELP :|=> INFO  \n");
	sendReply(fd, this->name + "NOTICE HELP :|=> KILL nickname reason \n");
}

void	Server::INVITEcmd(int fd,std::vector<std::string> &cmd)
{
	User	*inviter, *invitee;
	Channel	*channel;

	if (cmd.size() < 3)
		throw myException(ERR_NEEDMOREPARAMS);
	inviter = this->getUser(fd);
	if (!inviter)
		return;
	invitee = this->getUser(cmd[1]);
	if (!invitee)
		throw myException(ERR_NOSUCHNICK);
	channel = this->getChannel(cmd[2]);
	if (!channel)
		throw myException(ERR_NOSUCHCHANNEL);
	if (!channel->getMember(fd))
		throw myException(ERR_NOTONCHANNEL);
	if (channel->getMember(invitee->getFd()))
		throw myException(ERR_USERONCHANNEL);
	if (!channel->getOperator(fd))
		throw myException(ERR_CHANOPRIVSNEEDED);
	channel->addInvitee(invitee);
	sendReply(fd, this->name
		+ ft_tostring(RPL_INVITING) + " "
		+ inviter->getNickname() + " "
		+ invitee->getNickname() + " "
		+ channel->getName() + "\n");
	sendReply(fd, this->name
		+ "NOTICE @"
		+ channel->getName() + " :"
		+ inviter->getNickname() + " invited "
		+ invitee->getNickname() + " into channel "
		+ channel->getName() + "\n");
	sendReply(invitee->getFd(), ":" + inviter->getIdentifier() + " "
		+ "INVITE "
		+ invitee->getNickname() + " :"
		+ channel->getName() + "\n");
}

void	Server::kick(int fd, std::vector<std::string> &cmd)
{
	User		*op;
	User		*user;
	std::string	reply;

	if (cmd.size() < 3)
		throw myException(ERR_NEEDMOREPARAMS);
	Channel *channel = this->getChannel(cmd[1]);
	if (!channel)
		return throw myException(ERR_NOSUCHCHANNEL);
	op = channel->getOperator(fd);
	if (!op)
		return throw myException(ERR_CHANOPRIVSNEEDED);
	user = this->getUser(cmd[2]);
	if (!user || !channel->getMember(user->getFd()))
		return throw myException(ERR_NOSUCHNICK);
	reply = ":" + op->getIdentifier() + " "
		+ cmd[0] + " "
		+ channel->getName() + " "
		+ user->getNickname() + " :"
		+ (cmd.size() >= 4 ? cmd[3] : user->getNickname()) + "\n";
	channel->broadCastMessage(reply, fd);
	channel->removeMember(user->getFd());
	if (channel->getMembers().size() == 0)
		this->deleteChannel(channel->getName());
}

void	Server::part(int fd, std::vector<std::string> &cmd)
{
	Channel						*channel;
	User						*member;
	std::string					reply ;
	std::vector<std::string>	chans;
	size_t						i;

	if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);
	split(cmd[1], ',', chans);
	for (i = 0; i < chans.size(); i++)
	{
		channel = this->getChannel(chans[i]);
		if (!channel)
		{
			sendReply(fd,stringBuilder(9,this->getName().c_str()," ",
			ft_tostring(ERR_NOSUCHCHANNEL).c_str(), " ", this->getUser(fd)->getNickname().c_str()," ",chans[i].c_str(),
			" :",err_reply(ERR_NOSUCHCHANNEL).c_str()));
			continue;
		}
		member = channel->getMember(fd);
		if (!member)
			throw myException(ERR_NOTONCHANNEL);
		reply =  ":" + member->getIdentifier() + " "
			+ cmd[0] + " "
			+ channel->getName() + "\n";
		channel->broadCastMessage(reply, fd);
		channel->removeMember(fd);
		if (channel->getMembers().size() == 0)
			this->deleteChannel(channel->getName());
	}
}

void	Server::list(int fd, std::vector<std::string> &cmd)
{
	std::string					reply;
	std::vector<std::string>	channel;
	size_t						i = 0;
	Channel						*chans;

	reply = stringBuilder(3, this->getName().c_str(), " 321 ", " :Channel :Users  Name");
	sendReply(fd, reply);
	reply.clear();
	if (cmd.size() == 1)
	{
		std::map <std::string, Channel *>::iterator	it;

		for (it = this->channels.begin(); it != this->channels.end(); ++it)
		{
			if (!it->second->isPrivate())
				reply = stringBuilder(9, this->getName().c_str()," 322 ", this->getUser(fd)->getNickname().c_str(), " ", it->first.c_str(), " ",ft_tostring(it->second->getMembers().size()).c_str(), " :", it->second->getTopic().c_str());
			else
				reply = stringBuilder(8, this->getName().c_str()," 322 ", this->getUser(fd)->getNickname().c_str(), " ", it->first.c_str(), " ",ft_tostring(it->second->getMembers().size()).c_str(), " :");
			sendReply(fd, reply);
			reply.clear();
		}
	}
	else
	{
		split(cmd[1], ',', channel);
		while (i < channel.size())
		{	
			chans = this->getChannel(channel[i]);
			if (chans)
			{
				if (!chans->isPrivate())
					reply = stringBuilder(9, this->getName().c_str()," 322 ", this->getUser(fd)->getNickname().c_str(), " ", chans->getName().c_str(), " ",ft_tostring(chans->getMembers().size()).c_str(), " :", chans->getTopic().c_str());
				else
					reply = stringBuilder(8, this->getName().c_str()," 322 ", this->getUser(fd)->getNickname().c_str(), " ", chans->getName().c_str(), " ",ft_tostring(chans->getMembers().size()).c_str(), " :");
				sendReply(fd, reply);
				reply.clear();
			}
			i++;
		}
	}
	reply = stringBuilder(4, this->getName().c_str()," 323 ", this->getUser(fd)->getNickname().c_str(), " :End of /LIST");
	sendReply(fd, reply);
	reply.clear();
}

void	Server::channelModes(int fd, std::vector<std::string> & cmd) {
	Channel *			channel;
	User *				user;
	User *				op;
	std::stringstream	ss;
	bool				sign = true;
	size_t				argId = 3;

	channel = this->getChannel(cmd[1]);
	if (!channel)
		throw myException(ERR_NOSUCHCHANNEL);
	if (cmd.size() < 3 || cmd[2].empty()) {
		this->listChannelModes(channel, fd);
		return ;
	}
	op = channel->getOperator(fd);
	if (!op)
		throw myException(ERR_CHANOPRIVSNEEDED);
	for (size_t i = 0; i < cmd[2].length(); i++) {
		try {
			switch (cmd[2][i]) {
				case '+':
					sign = true;
					break;
				case '-':
					sign = false;
					break;
				case 'o':
					if (cmd.size() - 1 < argId)
						break;
					user = this->getUser(cmd[argId++]);
					if (!user)
						throw myException(this->name
							+ ft_tostring(ERR_NOSUCHNICK) + " "
							+ op->getNickname() + " "
							+ cmd[argId - 1] + " "
							+ err_reply(ERR_NOSUCHNICK) + "\n");
					if (sign) {
						channel->addOperator(user->getFd(), fd);
					} else {
						channel->removeOperator(user->getFd(), fd);
					}
					break;
				case 'p':
					channel->setPrivate(sign, fd);
					break;
				case 's':
					channel->setSecret(sign, fd);
					break;
				case 'i':
					channel->setInviteOnly(sign, fd);
					break;
				case 't':
					channel->setTopicSettable(sign, fd);
					break;
				case 'n':
					channel->setMemberChatOnly(sign, fd);
					break;
				case 'm':
					channel->setModerated(sign, fd);
					break;
				case 'l':
					if (cmd.size() - 1 < argId)
						throw myException(this->name
							+ ft_tostring(ERR_NEEDMOREPARAMS) + " "
							+ op->getNickname() + " "
							+ "MODE" + " "
							+ (sign ? "+" : "-") + cmd[2][i] + " "
							+ err_reply(ERR_NEEDMOREPARAMS) + "\n");
					if (sign) {
						int	limit;
						ss << cmd[argId++];
						ss >> limit;
						channel->setLimit(limit, fd);
					} else {
						channel->setLimit(-1, fd);
					}
					break;
				case 'b':
					if (cmd.size() - 1 < argId) {
						this->listChannelBans(channel, fd);
						break;
					}
					if (sign) {
						channel->addBan(cmd[argId++], fd);
					} else {
						channel->removeBan(cmd[argId++], fd);
					}
					break;
				case 'v':
					if (cmd.size() - 1 < argId)
						break;
					user = this->getUser(cmd[argId++]);
					if (!user)
						throw myException(this->name
							+ ft_tostring(ERR_NOSUCHNICK) + " "
							+ op->getNickname() + " "
							+ cmd[argId - 1] + " "
							+ err_reply(ERR_NOSUCHNICK) + "\n");
					if (sign) {
						channel->addModerator(user->getFd(), fd);
					} else {
						channel->removeModerator(user->getFd(), fd);
					}
					break;
				case 'k':
					if (cmd.size() - 1 < argId)
						break;
					if (sign) {
						channel->setKey(cmd[argId++], fd);
					} else {
						channel->setKey("", fd);
					}
					break;
				default:
					throw myException(this->name
						+ ft_tostring(ERR_UNKNOWNMODE) + " "
						+ op->getNickname() + " "
						+ cmd[2][i] + " "
						+ err_reply(ERR_UNKNOWNMODE) + "\n");
			}
		} catch (std::exception & e) {
			sendReply(fd, e.what());
		}
	}
}

void	Server::userModes(int fd, std::vector<std::string> & cmd) {
	User *	user;
	bool	sign = true;

	user = this->getUser(cmd[1]);
	if (!user)
		throw myException(ERR_NOSUCHCHANNEL);
	if (user->getFd() != fd)
		throw myException(ERR_USERSDONTMATCH);
	if (cmd.size() < 3 || cmd[2].empty()) {
		this->listUserModes(user, fd);
		return ;
	}
	for (size_t i = 0; i < cmd[2].length(); i++) {
		try {
			switch (cmd[2][i]) {
				case '+':
					sign = true;
					break;
				case '-':
					sign = false;
					break;
				case 'i':
					user->setVisibility(sign);
					sendReply(fd, ":" + user->getNickname() + " "
						+ "MODE" + " "
						+ user->getNickname() + " "
						+ (sign ? "+" : "-") + cmd[2][i] + "\n");
					break;
				default:
					throw myException(this->name
						+ ft_tostring(ERR_UMODEUNKNOWNFLAG) + " "
						+ user->getNickname() + " "
						+ err_reply(ERR_UMODEUNKNOWNFLAG) + "\n");
			}
		} catch (std::exception & e) {
			sendReply(fd, e.what());
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
	} catch (myException & e) {
		if (e.getERROR_NO() == ERR_USERSDONTMATCH) {
			sendReply(fd, this->getName()
				+ ft_tostring(e.getERROR_NO()) + " "
				+ this->getUser(fd)->getNickname() + " "
				+ e.what() + "\n");
		} else {
			sendReply(fd, this->getName()
				+ ft_tostring(e.getERROR_NO()) + " "
				+ this->getUser(fd)->getNickname() + " "
				+ cmd[1] + " "
				+ e.what() + "\n");
		}
	}
}

void	Server::names(int fd_u, std::vector<std::string> &cmd)
{
	std::vector<std::string>	channel;
	User						*user;
	Channel						*chan;

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
				sendReply(fd_u, this->getName()
					+ ft_tostring(RPL_ENDOFNAMES) + " "
					+ user->getNickname() + " "
					+ chan->getName() + " "
					+ reply(RPL_ENDOFNAMES) + "\n");
			}
			else
			{
				sendReply(fd_u, this->getName()
					+ ft_tostring(RPL_ENDOFNAMES) + " "
					+ user->getNickname() + " "
					+ channel[i] + " "
					+ reply(RPL_ENDOFNAMES) + "\n");
			}
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
		sendReply(fd_u, this->getName()
			+ ft_tostring(RPL_ENDOFNAMES) + " "
			+ user->getNickname() + " * "
			+ reply(RPL_ENDOFNAMES) + "\n");
	}
}


void    Server::QUITcmd(int fd, std::vector<std::string> & cmd)
{
	std::string reply;
	User *user;

	user = this->getUser(fd);
	if (!user)
	{
		if (!(user = this->getGuest(fd)))
			return ;
	}
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
			sendReply(fd, stringBuilder(3,":irc!~irc1337 381 ",
						user->getNickname().c_str()," :You are now an IRC operator"));
		}
		else
			throw myException(ERR_PASSWDMISMATCH);
	}
	
}

void	Server::KILLcmd(int fd,    std::vector<std::string> &cmd)
{
	User * user;

	user = this->getUser(fd);
	if (!user)
		return ;
	if (cmd.size() < 3)
		throw myException(ERR_NEEDMOREPARAMS);
	else if (!user->isOperator())
		throw myException(ERR_NOPRIVILEGES);
	else if (!this->getUser(cmd[1]))
		throw myException(ERR_NOSUCHNICK);
	sendReply(this->getUser(cmd[1])->getFd(),
		stringBuilder(5,":irc!~irc1337 ERROR Closing Link: (Killed ( ",// to change with the command error
				user->getNickname().c_str(), " ) ( " ,cmd[2].c_str()," ) )"));
	close(this->getUser(cmd[1])->getFd());
}

void	Server::topic(int fd, std::vector<std::string> &cmd)
{
	User		*user;
	Channel		*channel;
	std::string	reply;

	channel = this->getChannel(cmd[1]);
	user = this->getUser(fd);
	if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);
	if (!channel)
		throw myException(ERR_NOSUCHCHANNEL);
	if (cmd.size() == 2)
	{
		if (channel->getTopic().empty())
			throw myException(RPL_NOTOPIC);
		else
		{
			std::cout << "TOPIC : " << channel->getTopic() <<std::endl;
			reply = stringBuilder(7, this->getName().c_str(), "332 ", user->getNickname().c_str(),
			" ", channel->getName().c_str(), " :",channel->getTopic().c_str());
			sendReply(fd, reply);
		}
	}
	else
	{
		if (!channel->getOperator(fd))
			throw myException(ERR_NOPRIVILEGES);
		if (!channel->getMember(fd))
			throw myException(ERR_NOTONCHANNEL);
		channel->setTopic(cmd[2], fd);
		reply = stringBuilder(9, user->getNickname().c_str(), "!~", user->getUsername().c_str(),
						"@",user->getIpAddress().c_str(), " NOTICE TOPIC ", cmd[1].c_str(), " :", cmd[2].c_str());
		sendReply(fd, reply);
	}
}

void	Server::VERSIONcmd(int fd)
{
	sendReply(fd, stringBuilder(8,this->getName().c_str(),
				ft_tostring(RPL_VERSION).c_str()," ",
				this->getUser(fd)->getNickname().c_str()," ",
				this->getName().c_str(),
				this->getVersion().c_str(),":Beta Version"));
				
}

void	Server::TIMEcmd(int fd)
{
	time_t now = time(0);
	char* dt = ctime(&now);

	sendReply(fd, stringBuilder(8,this->getName().c_str(),
					ft_tostring(RPL_TIME).c_str()," ",
					this->getUser(fd)->getNickname().c_str()," ",
					this->getName().c_str()," :",dt));
}

void	Server::ADMINcmd(int fd)
{
	sendReply(fd, stringBuilder(7,this->getName().c_str(),
				ft_tostring(RPL_ADMINME).c_str(), " ",
				this->getUser(fd)->getNickname().c_str()," ",
				this->getName().c_str(),":Administrative info"));

	sendReply(fd, stringBuilder(5,this->getName().c_str(),
				ft_tostring(RPL_ADMINLOC1).c_str(), " ",
				this->getUser(fd)->getNickname().c_str(),
				" :The Server is in Morocco,Khouribga"));

	sendReply(fd, stringBuilder(5,this->getName().c_str(),
				ft_tostring(RPL_ADMINLOC2).c_str(), " ",
				this->getUser(fd)->getNickname().c_str(),
				" :The Server is being hosted in 1337 school and "
				"running by mbabela,hel-makh and ybensell"));

	sendReply(fd, stringBuilder(5,this->getName().c_str(),
				ft_tostring(RPL_ADMINEMAIL).c_str(), " ",
				this->getUser(fd)->getNickname().c_str(),
				" :hh@dontemailme.com"));
}

void Server::DataToFile()
{
   std::ofstream file("user.txt");

	std::map <int, User *>::iterator	  it ;
	std::map <int, User *> user_list = this->getUsers();
	for(it = user_list.begin(); it != user_list.end(); it++)
    {
        User *u;
        u = it->second;
        file << u->getFd()<< " " << u->getPostNumber() << " " << u->getIpAddress()  << " " << u->getNickname() << " " << u->getUsername() << std::endl;
    }
    file.close();
}

void	Server::welcomeReplay(int fd)
{
	User	*user;
	User	*bot;
	Player	*player;

	user = this->getUser(fd);
	time_t now = time(0);
	std::cout << "now : " << now <<  std::endl;
   	user->setLog(ctime(&now));
	// this->DataToFile();
	if (!user)
		return ;
	player = new Player();
	sendReply(fd, stringBuilder(9,this->getName().c_str(),"001 ",
				user->getNickname().c_str(),
				" :Welcome to the Internet Relay Network ",
				user->getNickname().c_str(),"!",
				user->getUsername().c_str(),
				"@",user->getIpAddress().c_str()));

	sendReply(fd, stringBuilder(7,this->getName().c_str(),"002 ",
				user->getNickname().c_str(),
				" :Your host is ",
				this->getName().c_str(), "running on version ",
				this->getVersion().c_str()));

	sendReply(fd, stringBuilder(5,this->getName().c_str(),"003 ",
				user->getNickname().c_str(),
				" :This server was created :",this->creationTime));
				
	if (user->isAuth())
	{
		bot = this->getUser("/lily");
		if (bot)
			sendReply(bot->getFd(), " : L_DAPET");
		std::cout << "looking for user " << std::endl;
		this->load_data();
		if (this->players_list.empty() || this->check_exist(user) == false)
			this->add_player(user);
		else
			this->link_data(user);
		std::cout << "number of Player : " << this->players_list.size() << std::endl;

	}
}

void	Server::fileTransfer(int fd,std::string &nick,std::vector<std::string> &vec)
{
	std::string rply;
 	User *reciever;
 	reciever = this->getUser(nick);
 	if (!reciever)

	rply = "NOTICE ";
	rply += nick;
	rply += " :";
	rply += 0x01;
	rply += "DCC GET ";
	rply += this->getUser(fd)->getNickname();
	rply += " ";
	rply += vec[2];
	rply += 0x01;
	sendReply(fd,rply);
}

// SEND USER FILE SIZE
void	Server::SENDcmd(int		fd, std::vector<std::string> &cmd)
{
	User *sender;
	User *receiver;
	std::stringstream ss;
 	__int64_t	fileSize;

	sender = this->getUser(fd);
	if (!sender)
		return ;
	receiver = this->getUser(cmd[1]);
	if (!receiver)
		throw myException(ERR_NOSUCHNICK);
	if (cmd.size() < 4)
		throw myException(ERR_NEEDMOREPARAMS);
	
	ss << cmd[4];
	ss >> fileSize;
	if (fileSize > 50000000)
	{
		sendReply (fd, stringBuilder(5,this->getName().c_str(),sender->getNickname().c_str(),
							" ERROR SEND ",cmd[2].c_str()," :file is too big"));
		return ;
	}
	sender->setFiles(cmd[2],fileSize);
	sendReply(receiver->getFd(),stringBuilder(6,this->getName().c_str(),receiver->getNickname().c_str(),
					" NOTICE SEND :**** ", sender->getNickname().c_str()," is sending the file : ",
					cmd[2].c_str()));
}

// ACCEPT FILE USER

void	Server::RESPONDcmd(int	fd, std::vector<std::string> &cmd)
{
	User *sender;

	sender = this->getUser(cmd[2]);
	if (!sender)
		throw myException(ERR_NOSUCHNICK);
	if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);

 
	std::map<std::string,size_t> files;
	std::map<std::string,size_t>::iterator it;

	files = sender->getFiles();
	it = files.find(cmd[1]);

	if (it == files.end())
	{
		sendReply (fd, stringBuilder(7,this->getName().c_str(),
					this->getUser(fd)->getNickname().c_str(),
					" ERROR SEND ",cmd[1].c_str()," :",sender->getNickname().c_str(),
					"Is not sending that file"));
		return ;
	}
	if (!cmd[0].compare("ACCEPT"))
	{
		std::stringstream ss;
		std::string fsize;

		ss << it->second;
		ss >> fsize;
		sendReply(sender->getFd(),stringBuilder(5,this->getName().c_str(),
						"NOTICE SEND ",cmd[1].c_str()," ACCEPT : ",
						this->getUser(fd)->getNickname().c_str(),
						" ACCEPTED the file. The sending Proccess will begin"));
		
		sendReply(fd,stringBuilder(8,this->getName().c_str(),"NOTICE SEND ",
						cmd[1].c_str()," " ,sender->getIpAddress().c_str(),
						" 5555 ",fsize.c_str()," ACCEPTED : file will start sending... "));
		sender->removeFile(cmd[1]);
	}
	else if (!cmd[0].compare("DECLINE"))
	{
		sendReply(sender->getFd(),stringBuilder(6,this->getName().c_str(),"NOTICE SEND " ,cmd[1].c_str(), " DECLINED : ",
					this->getUser(fd)->getNickname().c_str()," DECLINED the file"));
		sender->removeFile(cmd[1]);
	}
}
