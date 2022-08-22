/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 07:50:54 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/22 19:12:37 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/server.hpp"

void	close_server(Server & serv, int exit_status)
{
	for (int i = 0; i < serv.get_nfds(); i++)
	{
		if (serv.get_fds()[i].fd != 0)
			close(serv.get_fds()[i].fd);
	}
	exit(exit_status);
}

int main(int argc, char **argv)
{
	(void)	argc;
	(void)	argv;
	Server	serv = Server();

	if (!serv.Creat_socket()
		|| !serv.reusable_socket()
		|| !serv.nonblocking_socket()
		|| !serv.bind_socket()
		|| !serv.listen_from_socket())
		close_server(serv, EXIT_FAILURE);

	serv.poll_trait();

	do
	{
		std::cout << "Waiting for a poll . . . " << std::endl;
		int	rc = poll(serv.get_fds(), serv.get_nfds(), TIMEOUT);
		if (rc < 0)
		{
			std::cout << "FAILED to poll . . . " << std::endl;
			close_server(serv, EXIT_FAILURE);
		}
		if (rc == 0)
		{
			std::cout << "POLL : time out !" << std::endl;
			close_server(serv, EXIT_FAILURE);
		}
		for (int i = 0; i < serv.get_nfds(); i++)
		{
			if (serv.get_fds()[i].revents == 0)
				continue;
			if (serv.get_fds()[i].revents != POLLIN)
				std::cout << "Error ! revents : " << serv.get_fds()[i].revents << std::endl;
			if (serv.get_fds()[i].fd == serv.get_socket_fd())
			{
				if (!serv.accept_connection())
					close_server(serv, EXIT_FAILURE);
			}
			else
			{
				if (!serv.recv_send_msg(serv.get_fds()[i].fd))
				{
					close(serv.get_fds()[i].fd);
					for (int j = i; j < serv.get_nfds() - 1; j++)
					{
						serv.get_fds()[j].fd = serv.get_fds()[j + 1].fd;
						serv.get_fds()[j].events = serv.get_fds()[j + 1].events;
						serv.get_fds()[j].revents = serv.get_fds()[j + 1].revents;
					}
					serv.set_nfds(serv.get_nfds() - 1);
					i --;
				}
			}
		}
	} while (true);
	
	return (EXIT_SUCCESS);
}
