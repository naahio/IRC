/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 09:28:22 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/02 11:48:40 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOL_HPP
# define TOOL_HPP

# include <string>

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

#endif