/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 07:50:54 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/23 14:59:56 by hel-makh         ###   ########.fr       */
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
	std::cout << "Server Closed." << std::endl;
	exit(exit_status);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Error : " <<std::endl;
		std::cout << "Parmater format : ./ircserv <PORT> <PASSWORD>" <<std::endl;
		return (EXIT_FAILURE);
	}
	
	if(!isNumeric(argv[1]) && !strcmp(argv[2], PASSWORD))
	{
		std::cout << "error : <PORT> must be a <short int>" <<std::endl;
		return (EXIT_FAILURE);
	}

	Server serv = Server(std::stoi(argv[1]), argv[2]);

	if (!serv.Create_socket())
		return (EXIT_FAILURE);
	
	if(!serv.reusable_socket()
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
				if (!serv.accept_connections())
					close_server(serv, EXIT_FAILURE);
			}
			else
			{
				if (!serv.recv_send_msg(serv.get_fds()[i].fd))
				{
					close(serv.get_fds()[i].fd);
					for (int j = i; j < serv.get_nfds() - 1; j++)
					{
						memcpy(&serv.get_fds()[j], &serv.get_fds()[j + 1], sizeof(struct pollfd));
					}
					serv.set_nfds(serv.get_nfds() - 1);
					i --;
				}
			}
		}
	} while (true);
	
	return (EXIT_SUCCESS);
}
