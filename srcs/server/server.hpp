/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 13:13:06 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/24 13:12:19 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/poll.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <netinet/in.h>
# include <errno.h>
# include <iostream>
# include <unistd.h>
# include <cstring>
# include <iterator>
# include <map>
# include <fcntl.h>
# include <arpa/inet.h>

# include "../users/User.hpp"
# include "../tools/tool.hpp"
# include "../pars/msg.hpp"

# define BUFF_SIZE		1024
# define MAX_CONN		32
# define TIMEOUT		3 * 60 * 1000

class Server
{
	private:
		std::map <int, User>	users;
		// std::map <std::string, Channel>	channels;
		int						socket_fd;
		struct pollfd			fds[MAX_CONN];
		int						nfds;
		char					buffer[BUFF_SIZE];
		int						on;
		int						port;
		std::string				password;

		Server(void) {};

	public:
		Server(int port, std::string password);
		~Server(void);

		std::map <int, User> &	get_users(void);
		void					set_users(std::map <int, User> u_map);
		int						get_socket_fd(void) const;
		struct pollfd *			get_fds(void);
		int						get_nfds(void) const;
		void					set_nfds(int nfds);
		int						get_port(void) const;
		std::string const &		get_pass(void) const;

		int		Create_socket(void);
		int		reusable_socket(void);
		int		nonblocking_socket(void);
		int		bind_socket(void);
		int		listen_from_socket(void);
		void	poll_trait(void);
		bool	accept_connections(void);
		bool	recv_send_msg(int fd);
		
		
};

#endif