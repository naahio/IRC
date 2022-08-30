/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 08:54:42 by a                 #+#    #+#             */
/*   Updated: 2022/08/30 14:33:58 by ybensell         ###   ########.fr       */
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




// 	std::cout << "/**********************************\\" << std::endl;
// 	std::cout << "************** COMMANDS ************" << std::endl;
// 	for (size_t i = 0 ; i < this->commands.size(); i++)
// 	{
// 		// for (int j = 0 ; this->commands[i][j] != '\0' ; j++)
// 		// 	std::cout << std::hex << (int)this->commands[i][j] << std::endl;
// 		std::cout << this->commands[i] << std::endl;
// 	}
// 	std::cout << "/**********************************\\" << std::endl;



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
