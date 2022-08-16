/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 10:53:11 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/16 11:49:37 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/poll.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <netinet/in.h>
# include <errno.h>
# include <iostream>

# define SERVER_PORT 9999

# define TRUE           1
# define FALSE          0

int main(int argc, char **argv)
{
    int len              = 1;
    int rc               = 1;
    int on               = 1;
    int socket_fd        = -1;
    int new_sd           = -1;
    int desc_ready       = FALSE;
    int end_server       = FALSE;
    int compress_array   = FALSE;
    int close_conn;
    char buffer[100];
    struct sockaddr_in6 addr;
    int timeout;
    struct pollfd       fds[200];
    int     nfds = 1;
    int     current_size = 0;
    int     i;
    int     j;

    // creating socket v6

    socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        std::cout << "FAILED to create socket, errno " << errno << std::endl;
        exit (EXIT_FAILURE);
    }
    
    // 

    
}