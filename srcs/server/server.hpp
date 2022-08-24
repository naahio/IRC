/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 13:13:06 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/24 08:43:54 by mbabela          ###   ########.fr       */
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
# include <iterator>
# include <map>
# include <fcntl.h>
# include <arpa/inet.h>

# include "../users/User.hpp"
# include "../tools/tool.hpp"

# define BUFF_SIZE		1024
# define MAX_CONN		32
# define TIMEOUT		3 * 60 * 1000
# define PASSWORD		"password"

// template <typename T, class E>  
class Server
{
	private:
		std::map <int, User>			users;
		// std::map <std::string, Channel>	channels;
		int						socket_fd;
		struct pollfd			fds[MAX_CONN];
		int						nfds;
		char					buffer[BUFF_SIZE];
		int						on;
		int						port;
		std::string				password;

	public:
		Server(int port, std::string password);
		~Server();

		
		std::map <int, User> &	get_users();
		void					set_users(std::map <int, User> u_map);
		struct pollfd*			get_fds();
		int						get_socket_fd();
		int						get_nfds();
		void					set_nfds(int nfds);
		int						get_port();
		void					set_port(int port);
		std::string				get_pass();

		int		Creat_socket();
		int		reusable_socket();
		int		nonblocking_socket();
		int		bind_socket();
		int		listen_from_socket();
		void	poll_trait();
		bool	accept_connections();
		bool	recv_send_msg(int fd);
};

#endif