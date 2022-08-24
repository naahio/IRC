/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 09:28:22 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/24 13:50:35 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOOL_HPP
# define TOOL_HPP

# include "../server/server.hpp"

bool            isNumeric(std::string const &str);
std::string*    pre_cmd(std::string full_msg);

#endif