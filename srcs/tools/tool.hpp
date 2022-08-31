/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 09:28:22 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/31 15:21:01 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOL_HPP
# define TOOL_HPP

# include <string>

class myException : public std::exception {
	private:
		std::string	message;

	public:
		myException(std::string msg) : message(msg) {}
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