/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 15:32:31 by ybensell          #+#    #+#             */
/*   Updated: 2022/09/28 09:45:44 by hel-makh         ###   ########.fr       */
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

bool	wcMatch(std::string const & pat, std::string const & target) {
	size_t	i = 0;
	size_t	j = 0;

	for (i = 0; i < pat.size(); i++) {
		if (pat[i] != '*' && pat[i] != target[j])
			return (false);
		while (pat[i] == '*' && pat[i + 1] != target[j])
			j ++;
		if (pat[i] == '*')
			i ++;
		if (pat[i] != '*' && i < pat.size() && j < target.size() && pat[i] == target[j])
			j ++;
	}
	if (j != target.size())
		return (false);
	return (true);
}

std::string const &	removeDupWc(std::string & str) {
	std::string::iterator	it;

	for (it = str.begin(); it != str.end(); ++it) {
		if (*it == '*' && *(it + 1) == '*') {
			str.erase(it);
			--it;
		}
	}
	return (str);
}

t_ident	parseIdentifier(std::string identifier) {
	t_ident	ident;
	size_t	nick_pos = identifier.find("!");
	size_t	user_pos = identifier.find("@");
	
	if (nick_pos == std::string::npos && user_pos == std::string::npos)
		ident.nickname = identifier;
	else {
		if (nick_pos != std::string::npos)
			ident.nickname = identifier.substr(0, nick_pos);
		if (user_pos != std::string::npos) {
			ident.username = identifier.substr(nick_pos + 1, user_pos - nick_pos - 1);
			ident.hostname = identifier.substr(user_pos + 1, identifier.length());
		}
		else
			ident.username = identifier.substr(nick_pos + 1, identifier.length());
	}
	ident.nickname = ident.nickname == "" ? "*" : removeDupWc(ident.nickname);
	ident.username = ident.username == "" ? "*" : removeDupWc(ident.username);
	ident.hostname = ident.hostname == "" ? "*" : removeDupWc(ident.hostname);
	return (ident);
}

std::string const	ft_toLower(std::string str) {
	for (std::string::iterator it = str.begin(); it != str.end(); it++) {
		*it = tolower(*it);
	}
	return (str);
}

std::string const	ft_toString(int n) {
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
