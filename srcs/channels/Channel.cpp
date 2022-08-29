/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 14:19:45 by hel-makh          #+#    #+#             */
/*   Updated: 2022/08/29 09:53:51 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*************************[ Constructors/Destructors ]*************************/

Channel::Channel(std::string _name, User & _operator) {
	this->name = _name;
	this->op = &_operator;
}

/******************************[ Getters/Setters ]*****************************/

std::string const &	Channel::getName(void) const {
	return (this->name);
}

User *	Channel::getOperator(void) const {
	return (this->op);
}

std::map <std::string, User *> &	Channel::getMembers(void) {
	return (this->members);
}

void	Channel::setName(std::string _name) {
	this->name = _name;
}

void	Channel::setOperator(User *	_operator) {
	this->op = _operator;
}

/*****************************[ Member Functions ]*****************************/

void	Channel::addMember(User * member, std::string username) {
	this->members.insert(std::pair<std::string, User *>(username, member));
}

void	Channel::removeMember(std::string username) {
	std::map<std::string, User *>::iterator it;

	it = this->members.find(username);
	if (it != this->members.end()) {
		this->members.erase(it);
	}
}
