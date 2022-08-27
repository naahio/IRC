/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:31 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/27 16:58:26 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"

User::User(int _fd)
{
	this->fd		= _fd;
	this->username	= "";
	this->nickname	= "";
	this->password	= "";
	this->is_name	= false;
	this->is_nick	= false;
	this->is_pass	= false;
};

User::~User()
{
	close(this->fd);
}

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

void	User::setUsername(std::string _username)
{
	this->username = _username;
	this->is_name = true;
}

void	User::setNickname(std::string _nickname)
{
	this->nickname = _nickname;
	this->is_nick = true;
}

void	User::setPassword(std::string _password)
{
	this->password = _password;
	this->is_pass = true;
}

bool	User::isAuth(void)
{
	return (this->is_name && this->is_nick && this->is_pass);
}
