/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 08:54:42 by a                 #+#    #+#             */
/*   Updated: 2022/08/31 15:26:38 by ybensell         ###   ########.fr       */
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

Msg::Msg(std::string &full_msg, int sender)
{
	char *ptr;
	std::string tmp;
	
	this->sender = sender;
	ptr = strtok((char *)full_msg.c_str(),"\r\n");
	while (ptr != NULL)
	{
		this->commands.push_back(ptr);
		ptr = strtok(NULL,"\r\n");
	}

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

std::vector<std::string> Msg::getCommands(void)
{
	return this->commands;
}
// bool        Msg::check_syntax(std::string full_msg)
// {
	
// }
