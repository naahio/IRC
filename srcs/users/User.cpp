/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:31 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/28 09:51:21 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"

/*************************[ Constructors/Destructors ]*************************/

User::User(int _fd, char *ip, std::string post_name)
{
	this->postnumber	= post_name;
	this->fd			= _fd;
	this->username		= "";
	this->nickname		= "";
	this->servName		= "";
	this->fullName		= "";
	this->hostName		= "";
	this->msgRemainder	= "";
	this->ipAddress		= ip;
	this->registered	= false;
	this->connected		= false;
	this->visible		= true;
	this->isoperator	= false;
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
std::string const & User::getPostNumber(void) const
{
	return (this->postnumber);
}

std::string	const & User::getPassword(void) const
{
	return (this->password);
}

std::string const & User::getMsgRemainder(void) const
{
	return (this->msgRemainder);
}

std::map<std::string, Channel *> &	User::getChannels(void) {
	return (this->channels);
}

std::string const & User::getIpAddress(void) const
{
	return (this->ipAddress);
}

std::string const	User::getIdentifier(void) const {
	std::string	ident;

	ident = this->nickname + "!" + this->username + "@" + this->ipAddress;
	return (ident);
}

std::string const & User::getLog(void) const
{
	return (this->log);
}

Channel *	User::getChannel(std::string name) {
	std::map<std::string, Channel *>::iterator it;

	it = this->channels.find(name);
	if (it != this->channels.end()) {
		return (it->second);
	}
	return (NULL);
}

std::map<std::string,size_t> & User::getFiles(void)
{
	return (this->files);
}

void	User::setLog(std::string time)
{
	this->log = time;
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

void	User::setPassword(std::string &password)
{
	this->password = password;
}

void	User::setMsgRemainder(std::string &remaining)
{
	this->msgRemainder = remaining;
}

void	User::setRegistered(void)
{
	this->registered = true;
}

void	User::setConnected(void)
{
	this->connected = true;
}

void	User::setVisibility(bool option) {
	this->visible = option;
}

void	User::setIsOperator(void){
	this->isoperator = true;
}

void	User::setFiles(std::string filename,size_t fileSize)
{
	this->files.insert(std::pair<std::string,size_t>(filename,fileSize));
}
/*****************************[ Member Functions ]*****************************/

bool	User::isAuth(void) {
	return (!this->username.empty() && !this->nickname.empty());
}

bool	User::isRegistered(void)
{
	return this->registered;
}

bool	User::isConnected(void)
{
	return	this->connected;
}

bool	User::isVisible(void) {
	return	this->visible;
}

bool	User::isOperator(void){
	return this->isoperator;
}

void	User::removeFile(std::string file)
{
	std::map<std::string,size_t>::iterator it;

	it = this->files.find(file);
	this->files.erase(it);
}

void	User::joinChannel(Channel & channel, std::string name) {
	this->channels.insert(std::pair<std::string, Channel *>(ft_toLower(name), &channel));
}

void	User::leaveChannel(std::string name) {
	std::map<std::string, Channel *>::iterator it;

	it = this->channels.find(ft_toLower(name));
	if (it == this->channels.end()) {
		throw myException(ERR_NOTONCHANNEL);
	}
	this->channels.erase(it);
}

User*	User::operator = (const User& user) 
{ 
    this->fd			= user.fd;
    this->username		= user.username;
	this->nickname		= user.nickname;
	this->hostName		= user.hostName;
	this->ipAddress		= user.ipAddress;
	this->postnumber	= user.postnumber;
	this->connected		= user.connected;

	return (this);
}