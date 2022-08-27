/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 08:54:42 by a                 #+#    #+#             */
/*   Updated: 2022/08/24 13:15:18 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msg.hpp"

Msg::Msg(void)
{
	this->sender    = 0;
	this->full_msg  = "";
	this->cmd       = "";
	this->param_list = "";
}

Msg::Msg(std::string full_msg, int sender)
{
	this->sender = sender;
	this->full_msg = full_msg;
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

// bool        Msg::check_syntax(std::string full_msg)
// {
	
// }
