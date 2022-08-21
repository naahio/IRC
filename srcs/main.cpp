/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 07:50:54 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/21 13:20:23 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int main(int argc, char **argv)
{
   
    Server serv = Server();
    
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
        rc = poll(fds, nfds, TIMEOUT);
        if (rc < 0)
        {
            std::cout << "FAILED to poll . . . " << std::endl;
            break;
        }
        if (rc == 0)
        {
            std::cout << "POLL : time out !" << std::endl;
            break;
        }
        current_size = nfds;
        // i = 0;
        for (i=0; i < current_size; i++)
        {
            if (fds[i].revents == 0) // get the fd that listen and check if connected
                continue;
            if (fds[i].revents != POLLIN) // unexpected result
            {
                std::cout << "Error ! revents = : " << fds[i].revents << std::endl;
                end_Serverer = TRUE;
                break; 
            }
            if (fds[i].fd == socket_fd)
            {
                std::cout << "Socket readable and redy to get msgs . . . " << std::endl;
                do
                {
                    // check if we accept all the connection or not, and check the errno (all connection accepted)
                    new_fd = accept(socket_fd, NULL, NULL);
                    if (new_fd < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            std::cout << "FAILED at accepting connection ! " << std::endl;
                            end_Serverer = TRUE; 
                        }
                        break;
                    }
                    std::cout << "NEW Connection detected "<< new_fd << std::endl;
                    fds[nfds].fd = new_fd;
                    fds[nfds].events = POLLIN;
                    nfds++;
                }while (new_fd != -1);
            }
            else
            {
                std::cout << "A new fd (" << fds[i].fd << ") is readdable . . . " <<std::endl;
                close_conn = FALSE;
                do
                {
                    rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                    if (rc < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            std::cout << "FAILED to receive a msg !" << std::endl;
                            close_conn = TRUE;
                        }
                        break;
                    }
                    if (rc == 0)
                    {
                        std::cout << "Connection Closed . . . " << std::endl;
                        close_conn = TRUE;
                        break; 
                    }
                    len = rc;
                    std::cout << len << "bytes received !" << std::endl;
                    rc = send(fds[i].fd, buffer, len, 0);
                    if (rc < 0)
                    {
                        std::cout << "FAILED to send and answer to the client " << std::endl;
                        close_conn = TRUE;
                        break;
                    }
                } while (TRUE);
                if (close_conn)
                {
                    close (fds[i].fd);
                    fds[i].fd = -1;
                    compress_array = TRUE;
                }
            }
            // i++;
        }
        if (compress_array)
        {
            compress_array = FALSE;
            // i = 0;
            for (i = 0; i < nfds; i++)
            {
                if (fds[i].fd == -1)
                {
                    // j = i;
                    for (j = i; j < nfds-1; j++)
                    {
                        fds[j].fd = fds[j+1].fd;
                        // j++;
                    }
                    i--;
                    nfds--;
                }
                // i++;
            }
        }
    } while (end_Serverer == FALSE);
    
    // i = 0;
    for (i = 0; i < nfds; i++)
    {
        if (fds[i].fd >= 0)
            close (fds[i].fd);
        // i++;
    }
    return (0);
}