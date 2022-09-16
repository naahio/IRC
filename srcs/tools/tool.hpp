/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 09:28:22 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/16 11:08:20 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOL_HPP
# define TOOL_HPP

# include <string>
# include <sstream>
# include <vector>
# include <sys/socket.h>
# include <stdarg.h>
# include "replies.hpp"

class myException : public std::exception {
	private:
		std::string	message;

	public:
		myException(std::string msg) : message(msg) {}
		myException(int err_no) : message(err_reply(err_no)) {}
		virtual ~myException() throw() {}
		
		char const *	what() const throw() {
			return (message.c_str());
		}
};

bool				isNumeric(std::string const &str);
void				split(std::string const &s1, char delim, std::vector<std::string> &out);
int					paramsChecker(const std::string &param);

std::string const	ft_tostring(int n);
std::string const	stringBuilder(int n, ...);
void				sendReply(int fd, const std::string &reply);

//void	sendReply(int fd,const std::string &replay);
#endif