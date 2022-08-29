/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:31 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/29 12:01:40 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"

/*************************[ Constructors/Destructors ]*************************/

User::User(int _fd)
{
	this->fd		= _fd;
	this->username	= "";
	this->nickname	= "";
	this->password	= "";
	this->channel	= NULL;
};

User::~User()
{
	close(this->fd);
}

/******************************[ Getters/Setters ]*****************************/

int	User::getFd(void) const
{
	return (this->fd);
}

std::string const &	User::getUsername(void) const
{
	return (this->username);
}

std::string	const &	User::getNickname(void) const
{
	return (this->nickname);
}

std::string	const &	User::getPassword(void) const
{
	return (this->password);
}

Channel *	User::getChannel(void) const {
	return (this->channel);
}

void	User::setFd(int _fd)
{
	this->fd = _fd;
}

void	User::setUsername(std::string _username)
{
	this->username = _username;
}

void	User::setNickname(std::string _nickname)
{
	this->nickname = _nickname;
}

void	User::setPassword(std::string _password)
{
	this->password = _password;
}

/*****************************[ Member Functions ]*****************************/

bool	User::isAuth(void)
{
	return (!this->username.empty()
		&& !this->nickname.empty()
		&& !this->password.empty());
}

void	User::joinChannel(Channel * _channel) {
	if (_channel == NULL)
		return ;
	this->leaveChannel(_channel);
	this->channel = _channel;
	this->channel->addMember(this, this->username);
}

void	User::leaveChannel(Channel * _channel) {
	if (_channel == NULL || this->channel->getName() != _channel->getName())
		return ;
	this->channel = NULL;
	_channel->removeMember(this->username);
}
