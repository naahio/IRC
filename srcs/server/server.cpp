/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:44:13 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/14 12:03:06 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/socket.h>


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
*/

int bind(int socket_fd, const sockaddr *addr, socklen_t addrlen)
{

}

int main(int argc, char **argv)
{

    return (0);
}