/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 08:54:40 by a                 #+#    #+#             */
/* ************************************************************************** */
#ifndef MSG_HPP
# define MSG_HPP

#include <iostream>
#include <string>

class msg
{
private:
    std::string cmd;
    std::string para_list;
public:
    msg();
    ~msg();
};

#endif