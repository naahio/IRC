/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 09:28:22 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/31 11:52:49 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOL_HPP
# define TOOL_HPP

# include "../server/server.hpp"

bool			isNumeric(std::string const &str);
std::string *	pre_cmd(std::string full_msg);
void            split(std::string const &s1, char delim,
                         std::vector<std::string> &out);


#endif