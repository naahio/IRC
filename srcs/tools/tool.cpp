/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 15:32:31 by ybensell          #+#    #+#             */
/*   Updated: 2022/09/25 12:22:01 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "tool.hpp"

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

int	ft_toInt(std::string const & str)
{
	std::stringstream	ss;
	int	i;

	ss << str;
	ss >> i;
	return (i);
}

std::string const	stringBuilder(int n, ...) {
	va_list		ptr;
	std::string	str;

	va_start(ptr, n);
	for (int i = 0; i < n; i++) {
		str += va_arg(ptr, char *);
	}
	str += "\n";
	va_end(ptr);
	return (str);
}

void	sendReply(int fd,const std::string &reply)
{
	size_t total = 0;
	while (total != reply.length())
	{
		ssize_t nb = send(fd,reply.c_str() + total,reply.length() - total, 0);
		if (nb == -1)
			std::cout << "sending error" << std::endl; // to check later 
		total += nb;
	}
}
