/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 09:20:15 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/31 11:52:52 by ybensell         ###   ########.fr       */
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

void	split(std::string const &s1,char delim,
				std::vector<std::string> &out)
{
	std::stringstream X(s1);
	std::string T;

	while (std::getline(X,T,delim))
	{
		out.push_back(T);
	}
}

// std::string *	pre_cmd(std::string full_msg)
// {
// 	(void)full_msg;

	
// }
