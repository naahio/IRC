/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:44:13 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/14 13:38:14 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/socket.h>

struct sockaddr_in {
   sa_family_t    sin_family; /* address family: AF_INET */
   in_port_t      sin_port;   /* port in network byte order */
   struct in_addr sin_addr;   /* internet address */
};
//** socket : return (-1) in error or file descriptor : used to creat a socket
/*
    Domain :
        * AF_UNIX, AF_LOCAL : - Local communication
        * AF_INET           : - IPv4 Internet protocols
        * AF_INET6          : - IPv6 Internet protocols
        * AF_IPX            : - IPX Novell protocols
    type :
        * SOCK_STREAM   :   - Two-way reliable communication (TCP)
        * SOCK_DGRAM    :   - Connectionless, unreliable (UDP)
*/

int socket(int domain, int type, int protocol)
{

}

//** bind : assign an IP and port to the socket after creating it, return (-1) in error or (0) at succes
/*
    sock_fd :
        * the file descriptor returned by the socket() fct ;
    addr    :
        * a struct used to specify the address to assign to the socket, varies by protocol;
        * the use of 'sockaddr_in' bcs we will use an IP for the server.
    addlen  :
        * the addr size;
*/

int bind(int socket_fd, const sockaddr *addr, socklen_t addrlen)
{

}

//** listen : mark socket to accept the connections : ret (-1) or (0)
/*
    socket_fd   :
        * the file descriptor returned by the socket;
    backlog     :
        * the max number of connections allowed ;
*/

int listen(int socket_fd, int backlog)
{

}

//** accept : take an element from the socket ret (-1) or (0)
/*
    the arguments are the same as bind
*/

int accept(int sock_fd, sockaddr *addr, socklen_t *addrlen)
{

}



int main(int argc, char **argv)
{

    return (0);
}