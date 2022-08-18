/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:31 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/18 11:58:02 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "User.hpp"

User::User()
{
}

User::User(fd)
{
    this->fd = fd;
    this->username = NULL;
    this->nickname = NULL;
    this->password = NULL;
    this->is_name  = false;
    this->is_nick  = false;
    this->is_pass  = false;
    this->is_Auth  = false; 
}

User::~User()
{
}

