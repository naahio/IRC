/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:31 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/23 11:58:56 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"

User::User()
{
}

User::User(std::string ip, int fd)
{
    this->_IP      = ip;
	this->fd	   = fd;
    this->username = "";
    this->nickname = "";
    this->password = "";
    this->is_name  = false;
    this->is_nick  = false;
    this->is_pass  = false;
}

User::User(const User &u)
{
	*this = u;
}

User::~User()
{
}

std::string	User::get_username()
{
	return (this->username);
}

std::string	User::get_nickname()
{
	return (this->nickname);
}

std::string	User::get_password()
{
	return (this->password);
}

std::string	User::get_ip()
{
	return (this->_IP);
}

int		User::get_fd()
{
	return (this->fd);
}

void	User::set_username(std::string user_name)
{
	this->username = user_name;
}

void	User::set_nickname(std::string nick_name)
{
	this->nickname = nick_name;
}

void	User::set_password(std::string password)
{
	this->password = password;
}

bool	User::check_auth()
{
	return ((this->is_name && this->is_nick && this->is_pass));
}
