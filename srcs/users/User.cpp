/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:31 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/23 15:23:52 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"

User::User(std::string _ip, int _fd)
{
	this->ip		= _ip;
	this->fd		= _fd;
	this->username	= "";
	this->nickname	= "";
	this->password	= "";
	this->is_name	= false;
	this->is_nick	= false;
	this->is_pass	= false;
}

User::~User()
{
}

int		User::get_fd(void) const
{
	return (this->fd);
}

std::string	const &	User::get_ip(void) const
{
	return (this->ip);
}

std::string const &	User::get_username(void) const
{
	return (this->username);
}

std::string	const &	User::get_nickname(void) const
{
	return (this->nickname);
}

std::string	const &	User::get_password(void) const
{
	return (this->password);
}

void	User::set_username(std::string _username)
{
	this->username = _username;
	this->is_name = true;
}

void	User::set_nickname(std::string _nickname)
{
	this->nickname = _nickname;
	this->is_nick = true;
}

void	User::set_password(std::string _password)
{
	this->password = _password;
	this->is_pass = true;
}

bool	User::check_auth()
{
	return ((this->is_name && this->is_nick && this->is_pass));
}
