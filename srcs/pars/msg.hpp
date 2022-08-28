/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 08:54:40 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/28 15:38:09 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
# define MSG_HPP

# include <iostream>
# include <string>
# include <vector>

class Msg
{
	private:
		int			sender;

		std::string	full_msg;
		std::string	cmd;
		std::string	param_list;
		std::vector<std::string> parsedMsg;
		
	public:
		Msg(void);
		Msg(std::string full_msg, int sender);
		~Msg(void);

		std::string get_full_msg(void);
		std::string get_cmd(void);
		std::string get_param_list(void);
		int         get_sender(void);
		std::vector<std::string> getParsedMsg(void);

		bool        check_syntax(std::string full_msg);
};

#endif