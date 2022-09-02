/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:31 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/02 13:25:21 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"

/*************************[ Constructors/Destructors ]*************************/

User::User(int _fd)
{
	this->fd			= _fd;
	this->username		= "";
	this->nickname		= "";
	this->servName 		= "";
	this->fullName	 	= "";
	this->hostName   	= "";
	this->registered	= false;
}

User::~User()
{
	close(this->fd);
}

/******************************[ Getters/Setters ]*****************************/

int	User::getFd(void) const {
	return (this->fd);
}

std::string const &	User::getUsername(void) const {
	return (this->username);
}

std::string	const &	User::getNickname(void) const {
	return (this->nickname);
}

std::string const & User::getServerName(void) const
{
	return (this->servName);
}

std::string const & User::getHostName(void) const
{
	return (this->hostName);
}

std::string const & User::getFullName(void) const
{
	return (this->fullName);
}

std::map<std::string, Channel *> &	User::getChannels(void) {
	return (this->channels);
}

Channel *	User::getChannel(std::string name) {
	std::map<std::string, Channel *>::iterator it;

	it = this->channels.find(name);
	if (it != this->channels.end()) {
		return (it->second);
	}
	return (NULL);
}

void	User::setFd(int _fd) {
	this->fd = _fd;
}

void	User::setUsername(std::string _username) {
	this->username = _username;
}

void	User::setNickname(std::string _nickname) {
	this->nickname = _nickname;
}

void	User::setServerName(std::string _servName)
{
	this->servName = _servName;
}

void	User::setHostName(std::string _hostName)
{
	this->hostName = _hostName;
}

void	User::setFullName(std::string _fullName)
{
	this->fullName = _fullName;
}

void	User::setRegistered(void)
{
	this->registered = true;
}

/*****************************[ Member Functions ]*****************************/

bool	User::isAuth(void) {
	return (!this->username.empty() && !this->nickname.empty());
}

bool	User::isRegistered(void)
{
	return this->registered;
}

void	User::joinChannel(Channel & channel, std::string name) {
	this->channels.insert(std::pair<std::string, Channel *>(name, &channel));
}

void	User::leaveChannel(std::string name) {
	std::map<std::string, Channel *>::iterator it;

	it = this->channels.find(name);
	if (it == this->channels.end()) {
		throw myException(ERR_NOTONCHANNEL);
	}
	this->channels.erase(it);
}
