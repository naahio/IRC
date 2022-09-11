/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 10:13:51 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/11 09:52:00 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

class Server;

void    helps(int fd);
void	USERcmd(Msg &msg,std::vector<std::string> &cmd, Server &server);
void	NICKcmd(Msg &msg,std::vector<std::string> &cmd, Server &server);
void	PASScmd(Msg &msg,std::vector<std::string> &cmd, Server &server);
void	JOINcmd(Msg &msg,std::vector<std::string> &cmd, Server &server);
void	PRIVMSGcmd(Msg &msg,std::vector<std::string> &cmd, Server &server);

#endif