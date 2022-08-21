/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 07:50:54 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/21 14:08:41 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int main(int argc, char **argv)
{
   
    Server serv = Server();
    int    i;
    int    j;
    if (!serv.Creat_socket())
        exit (EXIT_FAILURE); 

    if (!serv.reusable_socket())
    {
        close (serv.get_socket_fd());
        exit (EXIT_FAILURE);
    }

    if (!serv.nonblocking_socket())
    {
        close (serv.get_socket_fd());
        exit (EXIT_FAILURE);
    }

    if (!serv.bind_socket())
    {
        close (serv.get_socket_fd());
        exit (EXIT_FAILURE);
    }

    if (!serv.listen_from_socket())
    {
        close (serv.get_socket_fd());
        exit (EXIT_FAILURE);
    }

    serv.poll_trait();

    do
    {
        std::cout << "Waiting for a poll . . . " << std::endl;
        serv.set_rc(poll(serv.get_fds(), serv.get_nfds(), TIMEOUT));
        if (serv.get_rc() < 0)
        {
            std::cout << "FAILED to poll . . . " << std::endl;
            break;
        }
        if (serv.get_rc() == 0)
        {
            std::cout << "POLL : time out !" << std::endl;
            break;
        }
        serv.set_current_size(serv.get_nfds());
        for (i=0; i < serv.get_current_size(); i++)
        {
            if (serv.get_fds()[i].revents == 0)
                continue;
            if (serv.get_fds()[i].revents != POLLIN)
            {
                std::cout << "Error ! revents = : " << serv.get_fds()[i].revents << std::endl;
                serv.set_end_Server(TRUE);
                break; 
            }
            if (serv.get_fds()[i].fd == serv.get_socket_fd())
                serv.accept_connect();
            else
            {
                serv.recv_send_msg();
                if (serv.get_close_conn())
                {
                    close (serv.get_fds()[i].fd);
                    serv.get_fds()[i].fd = -1;
                    serv.set_compress_array(TRUE);
                }
            }
        }
        if (serv.get_compress_array())
        {
            serv.set_compress_array(FALSE);
            for (i = 0; i < serv.get_nfds(); i++)
            {
                if (serv.get_fds()[i].fd == -1)
                {
                    for (j = i; j < serv.get_nfds() - 1; j++)
                        serv.get_fds()[j].fd = serv.get_fds()[j + 1].fd;
                    i--;
                    serv.set_nfds(serv.get_nfds() - 1);
                }
            }
        }
    } while (serv.get_end_Server() == FALSE);
    
    for (i = 0; i < serv.get_nfds(); i++)
    {
        if (serv.get_fds()[i].fd >= 0)
            close (serv.get_fds()[i].fd);
    }
    return (0);
}