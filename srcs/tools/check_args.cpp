/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 09:20:15 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/24 13:52:36 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "tool.hpp"
# include "../server/server.hpp"

bool	isNumeric(std::string const &str)
{
	std::string::const_iterator it = str.begin();
	while (it != str.end() && std::isdigit(*it))
		++it;
	return (!str.empty() && it == str.end());
}

// std::string *	pre_cmd(std::string full_msg)
// {
// 	(void)full_msg;

	
// }
