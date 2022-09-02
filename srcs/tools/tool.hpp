/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 09:28:22 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/02 12:47:51 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOL_HPP
# define TOOL_HPP

# include <string>
# include <vector>

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

bool			isNumeric(std::string const &str);
std::string *	pre_cmd(std::string full_msg);
void            split(std::string const &s1, char delim,
                         std::vector<std::string> &out);


#endif