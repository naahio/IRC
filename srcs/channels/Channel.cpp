/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 14:19:45 by hel-makh          #+#    #+#             */
/*   Updated: 2022/08/31 14:50:47 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*************************[ Constructors/Destructors ]*************************/

Channel::Channel(std::string _name) {
	std::string::iterator	it(_name.begin()), end(_name.end());
	
	if (_name[0] != '#') {
		throw myException("Invalid channel name: must start with '#'.");
	}
	for (; it != end; ++it) {
		if (isspace(*it)) {
			throw myException("Invalid channel name: contains illegal characters.");
		}
	}
	this->name = _name;
	this->topic = "";
	this->key = "";
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

std::map <int, User *> &	Channel::getOperators(void) {
	return (this->operators);
}

std::map <int, User *> &	Channel::getMembers(void) {
	return (this->members);
}

void	Channel::setName(std::string _name) {
	this->name = _name;
}

void	Channel::setTopic(std::string _topic) {
	this->topic = _topic;
}

void	Channel::setKey(std::string _key) {
	this->key = _key;
}

/*****************************[ Member Functions ]*****************************/

User *	Channel::getMember(int fd) {
	std::map<int, User *>::iterator	it;

	it = this->members.find(fd);
	if (it != this->members.end())
		return (it->second);
	return (NULL);
}

void	Channel::addOperator(int fd) {
	User *	member;
	
	member = this->getMember(fd);
	if (member == NULL)
		throw myException("Invalid username: user not in channel.");
	this->operators.insert(std::pair<int, User *>(fd, member));
}

void	Channel::removeOperator(int fd) {
	User *							member;
	std::map<int, User *>::iterator	it;
	
	member = this->getMember(fd);
	if (member == NULL) {
		throw myException("Invalid username: user not in channel.");
	}
	it = this->operators.find(fd);
	if (it == this->operators.end()) {
		throw myException("User is not an operator.");
	}
	this->operators.erase(it);
}

void	Channel::addMember(User * member, int fd, std::string key) {
	if (!this->key.empty() && this->key != key) {
		throw myException("Invalid channel key.");
	}
	if (this->members.insert(std::pair<int, User *>(fd, member)).second) {
		if (this->members.size() == 1) {
			this->addOperator(fd);
		}
	}
}

void	Channel::removeMember(int fd) {
	std::map<int, User *>::iterator	it;

	try {
		it = this->members.find(fd);
		if (it != this->members.end()) {
			this->removeOperator(fd);
			this->members.erase(it);
		}
	} catch (std::exception & e) {}
}
