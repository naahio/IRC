/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 14:19:45 by hel-makh          #+#    #+#             */
/*   Updated: 2022/09/14 12:59:06 by hel-makh         ###   ########.fr       */
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
	this->membersLimit = 0;
	this->_private = false;
	this->secret = false;
	this->memberChatOnly = false;
	this->inviteOnly = false;
	this->moderated = false;
	this->topicSettable = true;
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

bool	Channel::isTopicSettable(void) const {
	return (this->topicSettable);
}

std::map <int, User *> &	Channel::getMembers(void) {
	return (this->members);
}

std::vector <int> &	Channel::getOperators(void) {
	return (this->operators);
}

std::vector <int> &	Channel::getModerators(void) {
	return (this->moderators);
}

std::vector <int> &	Channel::getInvitees(void) {
	return (this->invitees);
}

void	Channel::setTopic(std::string _topic, int fd) {
	if (!this->topicSettable && !this->getOperator(fd))
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->topic = _topic;
}

void	Channel::setKey(std::string _key, int fd) {
	if (!this->getOperator(fd))
		throw myException(ERR_CHANOPRIVSNEEDED);
	if (!this->key.empty())
		throw myException(ERR_KEYSET);
	this->key = _key;
}

void	Channel::setLimit(size_t limit, int fd) {
	if (!this->getOperator(fd))
		throw myException(ERR_CHANOPRIVSNEEDED);
	if (limit)
		this->membersLimit = limit;
}

void	Channel::setPrivate(bool option, int fd) {
	if (!this->getOperator(fd))
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->_private = option;
}

void	Channel::setSecret(bool option, int fd) {
	if (!this->getOperator(fd))
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->secret = option;
}

void	Channel::setMemberChatOnly(bool option, int fd) {
	if (!this->getOperator(fd))
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->memberChatOnly = option;
}

void	Channel::setInviteOnly(bool option, int fd) {
	if (!this->getOperator(fd))
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->inviteOnly = option;
}

void	Channel::setModerated(bool option, int fd) {
	if (!this->getOperator(fd))
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->moderated = option;
}

void	Channel::setTopicSettable(bool option, int fd) {
	if (!this->getOperator(fd))
		throw myException(ERR_CHANOPRIVSNEEDED);
	this->topicSettable = option;
}

/*****************************[ Member Functions ]*****************************/

User *	Channel::getMember(int fd) {
	std::map<int, User *>::iterator	it;

	it = this->members.find(fd);
	if (it != this->members.end())
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

User *	Channel::getInvitee(int fd) {
	std::vector<int>::iterator	it;

	it = std::find(this->invitees.begin(), this->invitees.end(), fd);
	if (it != this->invitees.end())
		return (this->getMember(fd));
	return (NULL);
}

void	Channel::addMember(User * member, std::string _key) {
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

void	Channel::addOperator(int fd) {
	User *	member;
	
	member = this->getMember(fd);
	if (member == NULL) {
		throw myException(ERR_USERNOTINCHANNEL);
	}
	this->operators.push_back(fd);
}

void	Channel::removeOperator(int fd) {
	User *						member;
	std::vector<int>::iterator	it;
	
	member = this->getMember(fd);
	if (member == NULL) {
		throw myException(ERR_USERNOTINCHANNEL);
	}
	it = std::find(this->operators.begin(), this->operators.end(), fd);
	if (it != this->operators.end()) {
		this->operators.erase(it);
	}
}

void	Channel::addModerator(int fd) {
	User *	member;
	
	member = this->getMember(fd);
	if (member == NULL) {
		throw myException(ERR_USERNOTINCHANNEL);
	}
	this->moderators.push_back(fd);
}

void	Channel::removeModerator(int fd) {
	User *						member;
	std::vector<int>::iterator	it;
	
	member = this->getMember(fd);
	if (member == NULL) {
		throw myException(ERR_USERNOTINCHANNEL);
	}
	it = std::find(this->moderators.begin(), this->moderators.end(), fd);
	if (it != this->moderators.end()) {
		this->moderators.erase(it);
	}
}

void	Channel::addInvitee(int fd) {
	User *	member;
	
	member = this->getMember(fd);
	if (member == NULL) {
		throw myException(ERR_USERNOTINCHANNEL);
	}
	this->invitees.push_back(fd);
}

void	Channel::removeInvitee(int fd) {
	User *						member;
	std::vector<int>::iterator	it;
	
	member = this->getMember(fd);
	if (member == NULL) {
		throw myException(ERR_USERNOTINCHANNEL);
	}
	it = std::find(this->invitees.begin(), this->invitees.end(), fd);
	if (it != this->invitees.end()) {
		this->invitees.erase(it);
	}
}

void	Channel::broadCastMessage(std::string & message, int fd) {
	std::map<int, User *>::iterator	it;

	if ((this->memberChatOnly && !this->getMember(fd))
		|| (fd != -1 && this->moderated && !this->getModerator(fd))) {
		throw myException(ERR_CANNOTSENDTOCHAN);
	}
	for (it = this->members.begin(); it != this->members.end(); ++it) {
		if (fd != it->second->getFd())
			send(it->second->getFd(), message.c_str(), message.length(), 0);
	}
}
