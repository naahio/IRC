/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 14:19:45 by hel-makh          #+#    #+#             */
/*   Updated: 2022/09/28 10:19:18 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*************************[ Constructors/Destructors ]*************************/

Channel::Channel(std::string _name) {
	if (_name[0] != '#') {
		throw myException(ERR_NOSUCHCHANNEL);
	}
	for (std::string::iterator it = _name.begin(); it != _name.end(); ++it) {
		if (isspace(*it)) {
			throw myException(":Channel name contains illegal characters");
		}
	}
	this->name = _name;
	this->topic = "";
	this->key = "";
	this->creationTimestamp = std::time(0);
	this->membersLimit = 0;
	this->_private = false;
	this->secret = false;
	this->memberChatOnly = false;
	this->inviteOnly = false;
	this->moderated = false;
	this->topicSettableByOp = false;
}

/******************************[ Getters/Setters ]*****************************/

std::string const &	Channel::getName(void) const {
	return (this->name);
}

std::string const &	Channel::getTopic(void) const {
	return (this->topic);
}

std::string const &	Channel::getKey(void) const {
	return (this->key);
}

std::time_t	Channel::getCreationTimestamp(void) const {
	return (this->creationTimestamp);
}

size_t	Channel::getMembersLimit(void) const {
	return (this->membersLimit);
}

bool	Channel::isPrivate(void) const {
	return (this->_private);
}

bool	Channel::isSecret(void) const {
	return (this->secret);
}

bool	Channel::isMemberChatOnly(void) const {
	return (this->memberChatOnly);
}

bool	Channel::isInviteOnly(void) const {
	return (this->inviteOnly);
}

bool	Channel::isModerated(void) const {
	return (this->moderated);
}

bool	Channel::isTopicSettableByOp(void) const {
	return (this->topicSettableByOp);
}

std::map <int, User *> &	Channel::getMembers(void) {
	return (this->members);
}

std::map <int, User *> &	Channel::getInvitees(void) {
	return (this->invitees);
}

std::vector <int> &	Channel::getOperators(void) {
	return (this->operators);
}

std::vector <int> &	Channel::getModerators(void) {
	return (this->moderators);
}

std::vector <t_bans> &	Channel::getBans(void) {
	return (this->bans);
}

void	Channel::setTopic(std::string _topic, int fd) {
	User *		member;
	std::string	reply;

	member = this->getMember(fd);
	if (!member)
		throw myException(ERR_NOTONCHANNEL);
	if (!this->getOperator(fd) && this->topicSettableByOp)
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->topic = _topic;
	reply = ":" + member->getIdentifier()
		+ " TOPIC "
		+ this->getName() + " "
		+ ":" + _topic + "\n";
	this->broadCastMessage(reply);
}

void	Channel::setKey(std::string _key, bool option, int fd) {
	User *		op;
	std::string	reply;

	op = this->getOperator(fd);
	if (!op)
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->key = option ? _key : "";
	reply = ":" + op->getIdentifier()
		+ " MODE "
		+ this->getName() + " "
		+ (option ? "+" : "-") + "k " + _key + "\n";
	this->broadCastMessage(reply);
}

void	Channel::setLimit(size_t limit, int fd) {
	User *		op;
	std::string	reply;

	op = this->getOperator(fd);
	if (!op)
		throw myException(ERR_CHANOPRIVSNEEDED);
	if (!limit)
		return ;
	this->membersLimit = limit;
	reply = ":" + op->getIdentifier()
		+ " MODE "
		+ this->getName() + " "
		+ "+l " + ft_toString(limit) + "\n";
	this->broadCastMessage(reply);
}

void	Channel::setPrivate(bool option, int fd) {
	User *		op;
	std::string	reply;

	op = this->getOperator(fd);
	if (!op)
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->_private = option;
	reply = ":" + op->getIdentifier()
		+ " MODE "
		+ this->getName() + " "
		+ (option ? "+" : "-") + "p\n";
	this->broadCastMessage(reply);
}

void	Channel::setSecret(bool option, int fd) {
	User *		op;
	std::string	reply;

	op = this->getOperator(fd);
	if (!op)
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->secret = option;
	reply = ":" + op->getIdentifier()
		+ " MODE "
		+ this->getName() + " "
		+ (option ? "+" : "-") + "s\n";
	this->broadCastMessage(reply);
}

void	Channel::setMemberChatOnly(bool option, int fd) {
	User *		op;
	std::string	reply;

	op = this->getOperator(fd);
	if (!op)
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->memberChatOnly = option;
	reply = ":" + op->getIdentifier()
		+ " MODE "
		+ this->getName() + " "
		+ (option ? "+" : "-") + "n\n";
	this->broadCastMessage(reply);
}

void	Channel::setInviteOnly(bool option, int fd) {
	User *		op;
	std::string	reply;

	op = this->getOperator(fd);
	if (!op)
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->inviteOnly = option;
	reply = ":" + op->getIdentifier()
		+ " MODE "
		+ this->getName() + " "
		+ (option ? "+" : "-") + "i\n";
	this->broadCastMessage(reply);
}

void	Channel::setModerated(bool option, int fd) {
	User *		op;
	std::string	reply;

	op = this->getOperator(fd);
	if (!op)
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->moderated = option;
	reply = ":" + op->getIdentifier()
		+ " MODE "
		+ this->getName() + " "
		+ (option ? "+" : "-") + "m\n";
	this->broadCastMessage(reply);
}

void	Channel::setTopicSettableByOp(bool option, int fd) {
	User *		op;
	std::string	reply;

	op = this->getOperator(fd);
	if (!op)
		throw myException(ERR_CHANOPRIVSNEEDED);
	if (!this->getOperator(fd))
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->topicSettableByOp = option;
	reply = ":" + op->getIdentifier()
		+ " MODE "
		+ this->getName() + " "
		+ (option ? "+" : "-") + "t\n";
	this->broadCastMessage(reply);
}

/*****************************[ Member Functions ]*****************************/

User *	Channel::getMember(int fd) {
	std::map<int, User *>::iterator	it;

	it = this->members.find(fd);
	if (it != this->members.end())
		return (it->second);
	return (NULL);
}

User *	Channel::getInvitee(int fd) {
	std::map<int, User *>::iterator	it;

	it = this->invitees.find(fd);
	if (it != this->invitees.end())
		return (it->second);
	return (NULL);
}

User *	Channel::getOperator(int fd) {
	std::vector<int>::iterator	it;

	it = std::find(this->operators.begin(), this->operators.end(), fd);
	if (it != this->operators.end())
		return (this->getMember(fd));
	return (NULL);
}

User *	Channel::getModerator(int fd) {
	std::vector<int>::iterator	it;

	it = std::find(this->moderators.begin(), this->moderators.end(), fd);
	if (it != this->moderators.end())
		return (this->getMember(fd));
	return (NULL);
}

void	Channel::addMember(User * member, std::string _key) {
	if (this->isBanned(member)) {
		throw myException(ERR_BANNEDFROMCHAN);
	}
	if (this->inviteOnly && !this->getInvitee(member->getFd())) {
		throw myException(ERR_INVITEONLYCHAN);
	}
	if (!this->key.empty() && this->key != _key) {
		throw myException(ERR_BADCHANNELKEY);
	}
	if (this->membersLimit && this->members.size() >= this->membersLimit) {
		throw myException(ERR_CHANNELISFULL);
	}
	if (this->members.insert(std::pair<int, User *>(member->getFd(), member)).second) {
		member->joinChannel(*this, this->name);
		if (this->members.size() == 1) {
			this->addOperator(member->getFd());
		}
		if (this->inviteOnly && this->getInvitee(member->getFd())) {
			this->removeInvitee(member->getFd());
		}
	}
}

void	Channel::removeMember(int fd) {
	try {
		std::map<int, User *>::iterator	it;

		it = this->members.find(fd);
		if (it != this->members.end()) {
			this->removeOperator(fd);
			this->removeModerator(fd);
			this->removeInvitee(fd);
			it->second->leaveChannel(this->name);
			this->members.erase(it);
		}
	} catch (std::exception & e) {
		throw myException(std::string(e.what()));
	}
}

void	Channel::addInvitee(User * invitee) {
	this->invitees.insert(std::pair<int, User *>(invitee->getFd(), invitee));
}

void	Channel::removeInvitee(int fd) {
	std::map<int, User *>::iterator	it;

	it = this->invitees.find(fd);
	if (it != this->invitees.end()) {
		this->invitees.erase(it);
	}
}

void	Channel::addOperator(int fd, int opFd) {
	User *		member;
	User *		op;
	std::string	reply;
	
	member = this->getMember(fd);
	if (member == NULL) {
		throw myException(ERR_USERNOTINCHANNEL);
	}
	this->operators.push_back(fd);
	if (opFd != -1) {
		op = this->getOperator(opFd);
		if (!op)
			return ;
		reply = ":" + op->getIdentifier() + " "
			+ "MODE" + " "
			+ this->getName() + " "
			+ "+o" + " "
			+ member->getNickname() + "\n";
		this->broadCastMessage(reply);
	}
}

void	Channel::removeOperator(int fd, int opFd) {
	User *						member;
	User *						op;
	std::vector<int>::iterator	it;
	std::string					reply;
	
	member = this->getMember(fd);
	if (member == NULL) {
		throw myException(ERR_USERNOTINCHANNEL);
	}
	it = std::find(this->operators.begin(), this->operators.end(), fd);
	if (it != this->operators.end()) {
		if (opFd != -1) {
			op = this->getOperator(opFd);
			if (!op)
				return ;
			reply = ":" + op->getIdentifier() + " "
				+ "MODE" + " "
				+ this->getName() + " "
				+ "-o" + " "
				+ member->getNickname() + "\n";
			this->broadCastMessage(reply);
		}
		this->operators.erase(it);
	}
}

void	Channel::addModerator(int fd, int opFd) {
	User *		member;
	User *		op;
	std::string	reply;
	
	member = this->getMember(fd);
	if (member == NULL) {
		throw myException(ERR_USERNOTINCHANNEL);
	}
	this->moderators.push_back(fd);
	if (opFd != -1) {
		op = this->getOperator(opFd);
		if (!op)
			return ;
		reply = ":" + op->getIdentifier() + " "
			+ "MODE" + " "
			+ this->getName() + " "
			+ "+v" + " "
			+ member->getNickname() + "\n";
		this->broadCastMessage(reply);
	}
}

void	Channel::removeModerator(int fd, int opFd) {
	User *						member;
	User *						op;
	std::vector<int>::iterator	it;
	std::string					reply;
	
	member = this->getMember(fd);
	if (member == NULL) {
		throw myException(ERR_USERNOTINCHANNEL);
	}
	it = std::find(this->moderators.begin(), this->moderators.end(), fd);
	if (it != this->moderators.end()) {
		if (opFd != -1) {
			op = this->getOperator(opFd);
			if (!op)
				return ;
			reply = ":" + op->getIdentifier() + " "
				+ "MODE" + " "
				+ this->getName() + " "
				+ "-v" + " "
				+ member->getNickname() + "\n";
			this->broadCastMessage(reply);
		}
		this->moderators.erase(it);
	}
}

void	Channel::addBan(std::string const & banMask, int fd) {
	User *		mod;
	t_bans		ban;
	t_ident		ident;
	std::string reply;

	mod = this->getOperator(fd);
	if (!mod)
		throw myException(ERR_CHANOPRIVSNEEDED);
	ident = parseIdentifier(banMask);
	ban.banMask = ident.nickname + "!" + ident.username + "@" + ident.hostname;
	ban.banMod = mod->getNickname() + "!" + mod->getUsername() + "@" + mod->getIpAddress();
	ban.banTimestamp = std::time(0);
	if (!this->isBanned(ban.banMask)) {
		this->bans.push_back(ban);
	}
	reply = ":" + ban.banMod + " "
		+ "MODE" + " "
		+ this->getName() + " "
		+ "+b" + " "
		+ ban.banMask + "\n";
	this->broadCastMessage(reply);
}

void	Channel::removeBan(std::string const & banMask, int fd) {
	User *							op;
	std::vector<t_bans>::iterator	it;
	t_ident							ident, ident2;
	std::string 					reply;
	
	op = this->getOperator(fd);
	if (!op)
		throw myException(ERR_CHANOPRIVSNEEDED);
	ident = parseIdentifier(banMask);
	for (it = this->bans.begin(); it != this->bans.end(); ++it) {
		ident2 = parseIdentifier(it->banMask);
		if (ft_toLower(ident.nickname) == ft_toLower(ident2.nickname)
			&& ft_toLower(ident.username) == ft_toLower(ident2.username)
			&& ft_toLower(ident.hostname) == ft_toLower(ident2.hostname)) {
			this->bans.erase(it);
			reply = ":" + op->getIdentifier() + " "
				+ "MODE" + " "
				+ this->getName() + " "
				+ "-b" + " "
				+ banMask + "\n";
			this->broadCastMessage(reply);
			break ;
		}
	}
}

bool	Channel::isBanned(User * user) {
	std::vector<t_bans>::iterator	it;
	t_ident							ident;

	for (it = this->bans.begin(); it != this->bans.end(); ++it) {
		ident = parseIdentifier(it->banMask);
		if (wcMatch(ft_toLower(ident.nickname), ft_toLower(user->getNickname()))
			&& wcMatch(ident.username, user->getUsername())
			&& wcMatch(ident.hostname, user->getHostName())) {
			return (true);
		}
	}
	return (false);
}

bool	Channel::isBanned(std::string banMask) {
	std::vector<t_bans>::iterator	it;
	t_ident							ident, ident2;

	ident = parseIdentifier(banMask);
	for (it = this->bans.begin(); it != this->bans.end(); ++it) {
		ident2 = parseIdentifier(it->banMask);
		if (wcMatch(ft_toLower(ident2.nickname), ft_toLower(ident.nickname))
			&& wcMatch(ident2.username, ident.username)
			&& wcMatch(ident2.hostname, ident.hostname)) {
			return (true);
		}
	}
	return (false);
}

void	Channel::broadCastMessage(std::string const & message, int fd, bool everyone) {
	std::map<int, User *>::iterator	it;

	if (fd != -1 && ((this->memberChatOnly && !this->getMember(fd))
		|| (this->moderated && !this->getModerator(fd)))) {
		throw myException(ERR_CANNOTSENDTOCHAN);
	}
	for (it = this->members.begin(); it != this->members.end(); ++it) {
		if (everyone || (!everyone && it->second->getFd() != fd))
			sendReply(it->second->getFd(), message);
	}
}
