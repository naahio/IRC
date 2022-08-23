/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 08:54:40 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/22 10:24:21 by hel-makh         ###   ########.fr       */
/*                                                                            */
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