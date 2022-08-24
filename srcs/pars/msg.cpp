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

Msg::Msg()
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

Msg::~Msg()
{
}

std::string Msg::get_full_msg()
{
    return (this->full_msg);
}

std::string Msg::get_cmd()
{
    return (this->cmd);
}

std::string Msg::get_param_list()
{
    return (this->param_list);
}

int         Msg::get_sender()
{
    return (this->sender);
}

bool        Msg::check_sytaxe(std::string full_msg)
{
    
}
