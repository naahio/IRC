/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 09:28:22 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/28 09:38:28 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOL_HPP
# define TOOL_HPP

# include <iostream>
# include <ostream>
# include <cstddef>
# include <string>
# include <sstream>
# include <vector>
# include <sys/socket.h>
# include <stdarg.h>
# include "replies.hpp"

typedef struct s_ident {
	std::string	nickname;
	std::string	username;
	std::string	hostname;
}	t_ident;

class myException : public std::exception {
	private:
		int			err_no;
		std::string	message;

	public:
		myException(std::string msg) : message(msg) {}
		myException(int err_no) :  err_no(err_no), message(err_reply(err_no)) {}
		virtual ~myException() throw() {}
		
		char const *	what() const throw() {
			return (message.c_str());
		}
		int				getERROR_NO() {
			return (err_no);
		}
};

bool				isNumeric(std::string const &str);
void				split(std::string const &s1, char delim, std::vector<std::string> &out);
int					paramsChecker(const std::string &param);
bool				wcMatch(std::string const & pat, std::string const & target);
t_ident				parseIdentifier(std::string identifier);
std::string const	ft_toLower(std::string str);
std::string const	ft_toString(int n);
int					ft_toInt(std::string const & str);
std::string const	stringBuilder(int n, ...);
void				sendReply(int fd, const std::string &reply);

#endif