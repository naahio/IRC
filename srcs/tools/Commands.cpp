/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 10:13:49 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/28 11:43:07 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../server/server.hpp"

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
		+ ft_toString(RPL_NAMREPLY) + " "
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

	user = this->getUser(fd);
	if (!user)
		throw myException(ERR_NOTREGISTERED);
	if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);
	split(cmd[1],',',channels);
	if (cmd.size() > 2)
		split(cmd[2],',',keys);
	if (keys.size() < channels.size())		
		keys.resize(channels.size(),"");

	for (size_t i = 0 ; i < channels.size() ; i++)
	{
		try {
			Channel *chan = this->getChannel(channels[i]);
			if (chan && chan->getMember(fd))
				continue;
			Player	*pl = this->getPlayer(fd);
			if (chan) {
				if (pl)
					pl->add_Points(CHANNEL_POINT);
				chan->addMember(user, keys[i]);
			}
			else {
				if (pl)
					pl->add_Points(CREATED_CHANNEL_POINT);
				this->createChannel(channels[i], *user);
				chan = this->getChannel(channels[i]);
			}
			rply = ":" + user->getIdentifier() + " "
				+ cmd[0] + " "
				+ chan->getName() + "\n";
			chan->broadCastMessage(rply);
			sendChannelUsers(fd, chan, user, chan->getName());
			sendReply(fd, this->getName()
				+ ft_toString(RPL_ENDOFNAMES) + " "
				+ user->getNickname() + " "
				+ chan->getName() + " "
				+ reply(RPL_ENDOFNAMES) + "\n");
		}
		catch (myException &e) {
			sendReply(fd, this->getName()
				+ ft_toString(e.getERROR_NO()) + " "
				+ user->getNickname() + " "
				+ channels[i] + " "
				+ e.what() + "\n");
		}
	}
}

void	Server::PRIVMSGcmd(int fd, std::vector<std::string> &cmd, bool notice)
{
	User	*user, *target;
	Channel	*chan;
	
	user = this->getUser(fd);
	if (!user)
		throw myException(ERR_NOTREGISTERED);
	try {
		if (cmd.size() == 1)
			throw myException(ERR_NORECIPIENT);
		else if (cmd.size() == 2)
			throw myException(ERR_NOTEXTTOSEND);
		chan = this->getChannel(cmd[1]);
		target = this->getUser(cmd[1]);
		if (!target && !chan)
			throw myException(ERR_NOSUCHNICK);
		if (chan)
		{
			std::string reply;
			reply = ":" + user->getIdentifier() + " "
				+ cmd[0] + " "
				+ chan->getName() + " :"
				+ cmd[2] + "\n";
			chan->broadCastMessage(reply, user->getFd(), false);
		}
		else if (target)
		{
			std::vector<std::string> vec;
			if (ctcpMessage(cmd[2],vec))
				fileTransfer(fd,cmd[1],vec);
			sendReply(target->getFd(), ":" + user->getIdentifier() + " "
				+ cmd[0] + " "
				+ target->getNickname() + " :"
				+ cmd[2] + "\n");
		}
	} catch(myException & e) {
		if (!notice || e.getERROR_NO() == ERR_NORECIPIENT || e.getERROR_NO() == ERR_NOTEXTTOSEND) {
			sendReply(fd, this->getName()
				+ ft_toString(e.getERROR_NO()) + " "
				+ user->getNickname() + " "
				+ (cmd.size() > 2 ? (cmd[1] + " ") : "")
				+ e.what() + "\n");
		}
	}
}

void	Server::PASScmd(int fd, std::vector<std::string> &cmd)
{
	User *user;

	user = this->getGuest(fd);
	if (!user)
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
			welcomeReply(fd);
		}
		else
		{
			sendReply(fd,stringBuilder(5,this->getName().c_str()," ",ft_toString(ERR_PASSWDMISMATCH).c_str(),
			" PASS ",err_reply(ERR_PASSWDMISMATCH).c_str()));
			cmd[1] = "BAD PASSWORD";
			QUITcmd(fd,cmd);
		}
	}
}

void	Server::NICKcmd(int fd, std::vector<std::string> &cmd)
{
	User *user;

	if (!(user = this->getUser(fd)) && !(user = this->getGuest(fd)))
		return ;
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
		if (this->players_list.empty() || this->check_exist(user) == false)
			this->add_player(user);
		else
			this->link_data(user);
	}
	if (!user->isRegistered() && user->isAuth())
	{
		if (user->isConnected() && user->getPassword() == this->getPass())
		{
			user->setRegistered();
			this->addUser(fd,user);
			this->guests.erase(this->guests.find(fd));
			welcomeReply(fd);
		}
		else
		{
			sendReply(fd,stringBuilder(5,this->getName().c_str()," ",
			ft_toString(ERR_PASSWDMISMATCH).c_str(),
			" PASS ",err_reply(ERR_PASSWDMISMATCH).c_str()));
			cmd[1] = "BAD PASSWORD";
			QUITcmd(fd,cmd);
		}
	}
}

void	Server::HELPcmd(int fd)
{
	sendReply(fd, this->name + "NOTICE HELP :Use te following commands to Register or Log-In:\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> PASS <password>\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> NICK <nickname>\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> USER <username> <hostname> <servername> <realname>\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> OPER <user> <password>\n");
	sendReply(fd, this->name + "NOTICE HELP :Text commands:\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> PRIVMSG <receiver>{,<receiver>} <text to be sent>\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> NOTICE <nickname> <text>\n");
	sendReply(fd, this->name + "NOTICE HELP :Channel commands:\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> LIST [<channel>{,<channel>}\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> NAMES [<channel>{,<channel>}]\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> JOIN <channel>{,<channel>} [<key>{,<key>}]\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> PART <channel>{,<channel>}\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> MODE <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>] [<ban mask>]\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> MODE <nickname> {[+|-]|i|w|s|o}\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> TOPIC <channel> [<topic>]\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> INVITE <nickname> <channel>\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> KICK <channel> <user> [<comment>]\n");
	sendReply(fd, this->name + "NOTICE HELP :Operator commands:\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> KILL <nickname> <comment>\n");
	sendReply(fd, this->name + "NOTICE HELP :Server Informations commands:\n");
	sendReply(fd, this->name + "NOTICE HELP :|=> HELP\n");
	sendReply(fd, this->name + "NOTICE HELP :|=> VERSION\n");
	sendReply(fd, this->name + "NOTICE HELP :|=> TIME\n");
	sendReply(fd, this->name + "NOTICE HELP :|=> ADMIN\n");
	sendReply(fd, this->name + "NOTICE HELP :Other commands:\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> PONG <daemon> [<daemon2>]\n");
	sendReply(fd, this->name + "NOTICE HELP :|-> QUIT [<Quit message>]\n");
}

void	Server::INVITEcmd(int fd,std::vector<std::string> &cmd)
{
	User	*inviter, *invitee;
	Channel	*channel;

	inviter = this->getUser(fd);
	if (!inviter)
		throw myException(ERR_NOTREGISTERED);
	if (cmd.size() < 3)
		throw myException(ERR_NEEDMOREPARAMS);
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
		+ ft_toString(RPL_INVITING) + " "
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

void	Server::KICKcmd(int fd, std::vector<std::string> &cmd)
{
	User		*op;
	User		*user;
	Channel		*channel;
	std::string	reply;

	op = this->getUser(fd);
	if (!op)
		throw myException(ERR_NOTREGISTERED);
	if (cmd.size() < 3)
		throw myException(ERR_NEEDMOREPARAMS);
	try {
		channel = this->getChannel(cmd[1]);
		if (!channel)
			return throw myException(this->getName()
				+ ft_toString(ERR_NOSUCHCHANNEL) + " "
				+ op->getNickname() + " "
				+ cmd[1] + " "
				+ err_reply(ERR_NOSUCHCHANNEL) + "\n");
		op = channel->getOperator(fd);
		if (!op)
			return throw myException(this->getName()
				+ ft_toString(ERR_CHANOPRIVSNEEDED) + " "
				+ this->getUser(fd)->getNickname() + " "
				+ cmd[1] + " "
				+ err_reply(ERR_CHANOPRIVSNEEDED) + "\n");
		user = this->getUser(cmd[2]);
		if (!user || !channel->getMember(user->getFd()))
			return throw myException(this->getName()
				+ ft_toString(ERR_NOSUCHNICK) + " "
				+ op->getNickname() + " "
				+ cmd[2] + " "
				+ err_reply(ERR_NOSUCHNICK) + "\n");
		reply = ":" + op->getIdentifier() + " "
			+ cmd[0] + " "
			+ channel->getName() + " "
			+ user->getNickname() + " :"
			+ (cmd.size() >= 4 ? cmd[3] : op->getNickname()) + "\n";
		channel->broadCastMessage(reply, fd);
		channel->removeMember(user->getFd());
		if (channel->getMembers().size() == 0)
			this->deleteChannel(channel->getName());
	} catch(std::exception & e) {
		sendReply(fd, e.what());
	}
}

void	Server::PARTcmd(int fd, std::vector<std::string> &cmd)
{
	User						*user;
	Channel						*channel;
	std::vector<std::string>	channels;

	user = this->getUser(fd);
	if (!user)
		throw myException(ERR_NOTREGISTERED);
	if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);
	split(cmd[1], ',', channels);
	for (size_t i = 0; i < channels.size(); i++)
	{
		try {
			channel = this->getChannel(channels[i]);
			if (!channel)
				throw myException(ERR_NOSUCHCHANNEL);
			if (!channel->getMember(fd))
				throw myException(ERR_NOTONCHANNEL);
			std::string reply =  ":" + user->getIdentifier() + " "
				+ cmd[0] + " "
				+ channel->getName() + "\n";
			channel->broadCastMessage(reply, fd);
			channel->removeMember(fd);
			if (channel->getMembers().size() == 0)
				this->deleteChannel(channel->getName());
		} catch(myException & e) {
			sendReply(fd, ":" + user->getIdentifier() + " "
				+ ft_toString(e.getERROR_NO()) + " "
				+ user->getNickname() + " "
				+ (channel ? channel->getName() : channels[i]) + " "
				+ e.what() + "\n");
		}
	}
}

void	Server::LISTcmd(int fd, std::vector<std::string> &cmd)
{
	User						*user;
	std::vector<std::string>	channels;

	user = this->getUser(fd);
	if (!user)
		throw myException(ERR_NOTREGISTERED);
	sendReply(fd, this->getName()
		+ ft_toString(RPL_LISTSTART) + " "
		+ reply(RPL_LISTSTART) + "\n");
	if (cmd.size() == 1)
	{
		std::map <std::string, Channel *>::iterator	it;
		for (it = this->channels.begin(); it != this->channels.end(); ++it)
		{
			sendReply(fd, this->getName()
				+ ft_toString(RPL_LIST) + " "
				+ user->getNickname() + " "
				+ it->first + " "
				+ ft_toString(it->second->getMembers().size()) + " :"
				+ (it->second->isPrivate() ? "" : it->second->getTopic()) + "\n");
		}
	}
	else
	{
		split(cmd[1], ',', channels);
		for (size_t i = 0; i < channels.size(); i++)
		{	
			Channel *channel = this->getChannel(channels[i]);
			if (!channel)
				continue;
			sendReply(fd, this->getName()
				+ ft_toString(RPL_LIST) + " "
				+ user->getNickname() + " "
				+ channel->getName() + " "
				+ ft_toString(channel->getMembers().size()) + " :"
				+ (channel->isPrivate() ? "" : channel->getTopic()) + "\n");
		}
	}
	sendReply(fd, this->getName()
		+ ft_toString(RPL_LISTEND) + " "
		+ user->getNickname() + " "
		+ reply(RPL_LISTEND) + "\n");
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
							+ ft_toString(ERR_NOSUCHNICK) + " "
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
					channel->setTopicSettableByOp(sign, fd);
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
							+ ft_toString(ERR_NEEDMOREPARAMS) + " "
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
							+ ft_toString(ERR_NOSUCHNICK) + " "
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
					channel->setKey(cmd[argId++], sign, fd);
					break;
				default:
					throw myException(this->name
						+ ft_toString(ERR_UNKNOWNMODE) + " "
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
					user->setVisibility(!sign);
					sendReply(fd, ":" + user->getNickname() + " "
						+ "MODE" + " "
						+ user->getNickname() + " "
						+ (sign ? "+" : "-") + cmd[2][i] + "\n");
					break;
				default:
					throw myException(this->name
						+ ft_toString(ERR_UMODEUNKNOWNFLAG) + " "
						+ user->getNickname() + " "
						+ err_reply(ERR_UMODEUNKNOWNFLAG) + "\n");
			}
		} catch (std::exception & e) {
			sendReply(fd, e.what());
		}
	}
}

void	Server::MODEcmd(int fd, std::vector<std::string> & cmd) {
	if (!this->getUser(fd))
		throw myException(ERR_NOTREGISTERED);
	if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);
	try {
		if (cmd[1][0] == '#') {
			channelModes(fd, cmd);
		} else {
			userModes(fd, cmd);
		}
	} catch (myException & e) {
		sendReply(fd, this->getName()
			+ ft_toString(e.getERROR_NO()) + " "
			+ this->getUser(fd)->getNickname() + " "
			+ (e.getERROR_NO() == ERR_USERSDONTMATCH ? "" : cmd[1] + " ")
			+ e.what() + "\n");
	}
}

void	Server::NAMEScmd(int fd, std::vector<std::string> &cmd)
{
	std::vector<std::string>	channel;
	User						*user;
	Channel						*chan;

	user = this->getUser(fd);
	if (!user)
		throw myException(ERR_NOTREGISTERED);
	if (cmd.size() > 1)
		split(cmd[1],',',channel);
	if (channel.size() > 0)
	{
		for (size_t i = 0; i < channel.size(); i++)
		{
			chan = this->getChannel(channel[i]);
			if (chan && !chan->isPrivate() && !chan->isSecret())
				sendChannelUsers(fd, chan, user,channel[i]);
			sendReply(fd, this->getName()
				+ ft_toString(RPL_ENDOFNAMES) + " "
				+ user->getNickname() + " "
				+ (chan && !chan->isPrivate() && !chan->isSecret() ?
					chan->getName() : channel[i]) + " "
				+ reply(RPL_ENDOFNAMES) + "\n");
		}
	}
	else
	{
		std::map <std::string, Channel *>::iterator	  it ;
		std::map <std::string, Channel *> serverChannels =  this->getChannels();
		
		for (it = serverChannels.begin(); it != serverChannels.end(); it++)
		{
			if (!it->second->isPrivate() && !it->second->isSecret())
				sendChannelUsers(fd, it->second, user,it->second->getName());
		}
		sendReply(fd, this->getName()
			+ ft_toString(RPL_ENDOFNAMES) + " "
			+ user->getNickname() + " * "
			+ reply(RPL_ENDOFNAMES) + "\n");
	}
}


void    Server::QUITcmd(int fd, std::vector<std::string> & cmd)
{
	User		*user;
	std::string	reply;

	if (!(user = this->getUser(fd)) && !(user = this->getGuest(fd)))
		return ;
	sendReply(fd, "ERROR :Closing Link: "
		+ user->getIpAddress() + " (Quit :"
		+ (cmd[1].empty() ? user->getNickname() : cmd[1]) + ")\n");
	this->clientDisconnect(fd);
}

void	Server::OPERcmd(int fd, std::vector<std::string> &cmd)
{
	User											*user;
	std::map <std::string, std::string>::iterator	it;

	user = this->getUser(fd);
	if (!user)
		throw myException(ERR_NOTREGISTERED);
	if (cmd.size() < 3)
		throw myException(ERR_NEEDMOREPARAMS);
	it = this->getOperators().find(cmd[1]);
	if (it == this->getOperators().end())
		return ;
	if (it->second != cmd[2])
		throw myException(ERR_PASSWDMISMATCH);
	user->setIsOperator();
	sendReply(fd, this->name
		+ ft_toString(RPL_YOUREOPER) + " "
		+ user->getNickname() + " "
		+ reply(RPL_YOUREOPER) + "\n");
}

void	Server::KILLcmd(int fd, std::vector<std::string> &cmd)
{
	User	*op, *user;

	op = this->getUser(fd);
	if (!op)
		throw myException(ERR_NOTREGISTERED);
	if (cmd.size() < 3)
		throw myException(ERR_NEEDMOREPARAMS);
	try {
		if (!op->isOperator())
			throw myException(ERR_NOPRIVILEGES);
		user = this->getUser(cmd[1]);
		if (!user)
			throw myException(ERR_NOSUCHNICK);
		sendReply(user->getFd(), ":" + op->getIdentifier() + " "
			+ "ERROR Closing Link: " + this->name
			+ "(Killed (" + op->getNickname()
			+ " (" + cmd[2] + ")))\n");
		this->clientDisconnect(user->getFd());
	} catch (myException &e) {
		sendReply(fd, this->getName()
			+ ft_toString(e.getERROR_NO()) + " "
			+ op->getNickname() + " "
			+ (user ? user->getNickname() + " " : "")
			+ e.what() + "\n");
	}
}

void	Server::TOPICcmd(int fd, std::vector<std::string> &cmd)
{
	User	*op;
	Channel	*channel;

	op = this->getUser(fd);
	if (!op)
		throw myException(ERR_NOTREGISTERED);
	if (cmd.size() < 2)
		throw myException(ERR_NEEDMOREPARAMS);
	try {
		channel = this->getChannel(cmd[1]);
		if (!channel)
			throw myException(ERR_NOSUCHCHANNEL);
		if (cmd.size() > 2) {
			channel->setTopic(cmd[2], fd);
			return ;
		}
		if (channel->getTopic().empty()) {
			sendReply(fd, this->getName()
				+ ft_toString(RPL_NOTOPIC) + " "
				+ op->getNickname() + " "
				+ channel->getName() + " "
				+ reply(RPL_NOTOPIC) + "\n");
		} else {
			sendReply(fd, this->getName()
				+ ft_toString(RPL_TOPIC) + " "
				+ op->getNickname() + " "
				+ channel->getName() + " :"
				+ channel->getTopic() + "\n");
		}
	} catch (myException &e) {
		sendReply(fd, this->getName()
			+ ft_toString(e.getERROR_NO()) + " "
			+ op->getNickname() + " "
			+ (channel ? channel->getName() : cmd[1]) + " "
			+ e.what() + "\n");
	}
}

void	Server::VERSIONcmd(int fd)
{
	User *	user;

	if (!(user = this->getUser(fd)) && !(user = this->getGuest(fd)))
		return ;
	sendReply(fd, this->name
		+ ft_toString(RPL_VERSION) + " "
		+ (user->getNickname().empty() ? "*" : user->getNickname()) + " "
		+ this->version.substr(0, this->version.length() - 1) + ". "
		+ this->name.substr(1) + ":Beta Version\n");
}

void	Server::TIMEcmd(int fd)
{
	User *	user;
	time_t	now = time(0);
	char	*dt = ctime(&now);

	if (!(user = this->getUser(fd)) && !(user = this->getGuest(fd)))
		return ;
	sendReply(fd, this->name
		+ ft_toString(RPL_TIME) + " "
		+ (user->getNickname().empty() ? "*" : user->getNickname()) + " "
		+ this->name.substr(1)
		+ ft_toString(now) + " :"
		+ dt);
}

void	Server::ADMINcmd(int fd)
{
	User *	user;

	if (!(user = this->getUser(fd)) && !(user = this->getGuest(fd)))
		return ;
	sendReply(fd, this->name
		+ ft_toString(RPL_ADMINME) + " "
		+ (user->getNickname().empty() ? "*" : user->getNickname()) + " "
		+ reply(RPL_ADMINME) + "\n");
	sendReply(fd, this->name
		+ ft_toString(RPL_ADMINLOC1) + " "
		+ (user->getNickname().empty() ? "*" : user->getNickname()) + " :"
		+ this->name + "is in Khouribga, Morocco\n");
	sendReply(fd, this->name
		+ ft_toString(RPL_ADMINLOC1) + " "
		+ (user->getNickname().empty() ? "*" : user->getNickname()) + " :"
		+ this->name + "is being hosted in 1337 school"
		+ " and running by mbabela, hel-makh and ybensell\n");
	sendReply(fd, this->name
		+ ft_toString(RPL_ADMINEMAIL) + " "
		+ (user->getNickname().empty() ? "*" : user->getNickname()) + " :hh@dontemailme.com\n");
}

void	Server::welcomeReply(int fd)
{
	User	*user;

	user = this->getUser(fd);
	if (!user)
		return ;
	time_t now = time(0);
	user->setLog(ctime(&now));
	sendReply(fd, this->name + "001 "
		+ user->getNickname() + " :Welcome to the Internet Relay Network "
		+ user->getIdentifier() + "\n");
	sendReply(fd, this->name + "002 "
		+ user->getNickname() + " :Your host is "
		+ this->name.substr(1) + "running on version "
		+ this->getVersion() + "\n");
	sendReply(fd, this->name + "003 "
		+ user->getNickname() + " :This server was created "
		+ this->creationTime);
	if (user->isAuth())
	{
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
	User		*sender, *receiver;
	std::string reply;

	sender = this->getUser(fd);
 	receiver = this->getUser(nick);
 	if (!sender || !receiver)
		return;
	reply = "NOTICE ";
	reply += nick;
	reply += " :";
	reply += 0x01;
	reply += "DCC GET ";
	reply += sender->getNickname();
	reply += " ";
	reply += vec[2];
	reply += 0x01;
	sendReply(fd, reply);
}

void	Server::SENDcmd(int fd, std::vector<std::string> &cmd)
{
	User *sender;
	User *receiver;
	std::stringstream ss;
 	__int64_t	fileSize;

	sender = this->getUser(fd);
	if (!sender)
		throw myException(ERR_NOTREGISTERED);
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

void	Server::RESPONDcmd(int fd, std::vector<std::string> &cmd)
{
	User	*sender, *receiver;

	receiver = this->getUser(fd);
	if (!receiver)
		throw myException(ERR_NOTREGISTERED);
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
