/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 08:54:42 by ybensell          #+#    #+#             */
/*   Updated: 2022/09/16 14:23:09 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msg.hpp"

Msg::Msg(void)
{
	this->sender    = 0;
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


int	Msg::getSender(void)
{
	return (this->sender);
}


std::vector<std::string> &Msg::getCommands(void)
{
	return this->commands;
}
// bool        Msg::check_syntax(std::string full_msg)
// {
	
// }
