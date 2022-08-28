/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 08:54:42 by a                 #+#    #+#             */
/*   Updated: 2022/08/28 15:39:03 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msg.hpp"
#include <sstream>

Msg::Msg(void)
{
	this->sender    = 0;
	this->full_msg  = "";
	this->cmd       = "";
	this->param_list = "";
}

Msg::Msg(std::string full_msg, int sender)
{
	std::stringstream s(full_msg);
	std::string tmp;

	this->sender = sender;
	this->full_msg = full_msg;

	while (std::getline(s,tmp,' '))
		this->parsedMsg.push_back(tmp);
	this->cmd = this->parsedMsg[0];
}


Msg::~Msg(void)
{
}

std::string	Msg::get_full_msg(void)
{
	return (this->full_msg);
}

std::string	Msg::get_cmd(void)
{
	return (this->cmd);
}

std::string	Msg::get_param_list(void)
{
	return (this->param_list);
}

int	Msg::get_sender(void)
{
	return (this->sender);
}

std::vector<std::string> Msg::getParsedMsg(void)
{
	return this->parsedMsg;
}
// bool        Msg::check_syntax(std::string full_msg)
// {
	
// }
