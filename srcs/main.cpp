/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 07:50:54 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/28 09:49:21 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./server/server.hpp"

void	close_server(Server & serv, int exit_status)
{
	close(serv.getSocketFd());
	serv.~Server();
	exit(exit_status);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Parameter format : ./ircserv <PORT> <PASSWORD>" <<std::endl;
		return (EXIT_FAILURE);
	}
	if(!isNumeric(argv[1]))
	{
		std::cout << "Error : Port error !" <<std::endl;
		exit (EXIT_FAILURE);
	}

	Server serv = Server(atoi(argv[1]), argv[2]);

	if (!serv.Create_socket())
		return (EXIT_FAILURE);
	
	if(!serv.reusable_socket()
		|| !serv.nonblocking_socket()
		|| !serv.bind_socket()
		|| !serv.listen_from_socket())
		close_server(serv, EXIT_FAILURE);

	serv.poll_trait();

	if (serv.getPlayers_List().empty())
		serv.load_data();
	std::cout << "number of existing Player : " << serv.getPlayers_List().size() << std::endl;
	do
	{
		std::cout << "Waiting for a poll . . . " << std::endl;
		int	rc = poll(serv.getFds(), serv.getNfds(), TIMEOUT);
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
		for (int i = 0; i < serv.getNfds(); i++)
		{
			if (serv.getFds()[i].revents == 0)
				continue;
			if (serv.getFds()[i].fd == serv.getSocketFd())
			{
				if (!serv.accept_connections())
					close_server(serv, EXIT_FAILURE);
			}
			else
			{
				if (!serv.recv_send_msg(serv.getFds()[i].fd))
				{
					serv.clientDisconnect(serv.getFds()[i].fd);
					for (int j = i; j < serv.getNfds(); j++)
					{
						memcpy(&serv.getFds()[j], &serv.getFds()[j + 1], sizeof(struct pollfd));
					}
					serv.setNfds(serv.getNfds() - 1);
					i --;
				}
			}
		}
	} while (true);
	
	return (EXIT_SUCCESS);
}
