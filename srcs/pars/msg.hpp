/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 08:54:40 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/04 16:13:34 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
# define MSG_HPP

# include <iostream>
# include <string>
# include <vector>
# include <sstream>
# include <cstring>

class Msg
{
	private:
		int			sender;
		std::vector<std::string> commands;

		
	public:
		Msg(void);
		Msg(std::string  & full_msg, int sender);
		~Msg(void);

		int         getSender(void);
		std::vector<std::string> &getCommands(void);

		bool        check_syntax(std::string full_msg);
};

#endif