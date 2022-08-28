/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:31 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/28 17:27:49 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"

User::User(int _fd)
{
	this->fd		 = _fd;
	this->username	 = "";
	this->nickname	 = "";
	this->password	 = "";
	this->servername = "";
	this->hostname   = "";
	this->is_name	 = false;
	this->is_nick	 = false;
	this->is_pass	 = false;
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

std::string const & User::getServerName(void) const
{
	return this->servername;
}

std::string const & User::getHostName(void) const
{
	return this->hostname;
}

std::string const & User::getFullName(void) const
{
	return this->fullName;
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

void	User::setServerName(std::string servername)
{
	this->servername = servername;
}

void	User::setHostName(std::string hostname)
{
	this->hostname = hostname;
}

void	User::setFullName(std::string fullName)
{
	this->fullName = fullName;
}

bool	User::isAuth(void)
{
	return (this->is_name && this->is_nick && this->is_pass);
}
