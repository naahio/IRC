/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 10:53:11 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/16 13:59:40 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "serv.hpp"

int main(int argc, char **argv)
{
    int len              = TRUE;
    int rc               = TRUE;
    int on               = TRUE;
    int socket_fd        = F_INIT;
    int new_sd           = F_INIT;
    int desc_ready       = FALSE;
    int end_server       = FALSE;
    int compress_array   = FALSE;
    int close_conn;
    char buffer[BUFF_SIZE];
    struct sockaddr_in6 addr;
    int timeout;
    struct pollfd       fds[POLL_SIZE];
    int     nfds = TRUE;
    int     current_size = FALSE;
    int     i;
    int     j;

    // creating socket v6

    socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        std::cout << "FAILED to create socket, errno : " << errno << std::endl;
        exit (EXIT_FAILURE);
    }
    
    // make the socket fd reuseable : 

    rc = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    if (rc < 0)
    {
        std::cout << "FAILED at set_sock_opt, errno : " << errno << std::endl;
        close (socket_fd);
        exit (EXIT_FAILURE);
    }
    
    // hadle the part of makingthe spocket nonblocking :
    
    rc = ioctl(socket_fd, FIONBIO, (char *)&on);
    if (rc < 0)
    {
        std::cout << "FAILED at making socket nonblocking, errno : " <<  errno << std::endl;
        close (socket_fd);
        exit (EXIT_FAILURE);
    }
    
    // assign a PORT and an IP to the socket : 

    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    addr.sin6_port = htons(SERVER_PORT);
    rc = bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (rc < 0)
    {
        std::cout << "FAILED at binding the socket, errno : " <<  errno << std::endl;
        close (socket_fd);
        exit (EXIT_FAILURE);
    }

    // set the socket to listen : 
    
    rc = listen(socket_fd, MAX_CONN);
    if (rc < 0)
    {
        std::cout << "FAILED at listing, errno : " << errno << std::endl;
        close (socket_fd);
        exit (EXIT_FAILURE);
    }

    //

}