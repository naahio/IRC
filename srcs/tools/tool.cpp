/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 15:32:31 by ybensell          #+#    #+#             */
/*   Updated: 2022/09/13 11:11:47 by hel-makh         ###   ########.fr       */
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
		if (!T.empty())
			out.push_back(T);
	}
}

int	paramsChecker(const std::string &param)
{
	if ((param.find_first_of(" \r\n\v\f\r\'\",*?!@.") != std::string::npos)
		|| param[0] == '$' || param[0] == ':'
		|| param[0] == '#' || param[0] == '&')
		return 0;
	return 1;
}

std::string const	ft_tostring(int n) {
	std::stringstream	ss;

	ss << n;
	return (ss.str());
}

std::string const	stringBuilder(int n, ...) {
	va_list		ptr;
	std::string	str;

	va_start(ptr, n);
	for (int i = 0; i < n; i++) {
		str += va_arg(ptr, char *);
		if (i < n -1) {
			str += " ";
		}
	}
	str += "\n";
	va_end(ptr);
	return (str);
}
