/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 14:19:45 by hel-makh          #+#    #+#             */
/*   Updated: 2022/08/27 18:03:00 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string _name, User * _operator) {
	this->name = _name;
	this->op = _operator;
}

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
