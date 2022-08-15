/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a <a@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:44:13 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/15 10:51:58 by a                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <sys/socket.h>    //=> Socket fct
# include <netinet/in.h>    //=> For sockaddr_in
# include <cstdlib>         //=> exit()
# include <iostream>        //std lib
# include <unistd.h>

// struct sockaddr_in {
//    sa_family_t    sin_family; /* address family: AF_INET */
//    in_port_t      sin_port;   /* port in network byte order */
//    struct in_addr sin_addr;   /* internet address */
// };

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

// int socket(int domain, int type, int protocol)
// {

// }

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

// int bind(int socket_fd, const sockaddr *addr, socklen_t addrlen)
// {

// }

//** listen : mark socket to accept the connections : ret (-1) or (0)
/*
    socket_fd   :
        * the file descriptor returned by the socket;
    backlog     :
        * the max number of connections allowed ;
*/

// int listen(int socket_fd, int backlog)
// {

// }

//** accept : take an element from the socket ret (-1) or (0)
/*
    the arguments are the same as bind
*/

// int accept(int sock_fd, sockaddr *addr, socklen_t *addrlen)
// {

// }



int main(int argc, char **argv)
{
    int sock_fd;
    int bind_fd;
    int listen_fd;
    int connection;
    char *buff;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0); // <<<< SOCKET
    if (sock_fd == -1)
    {
        std::cout << "Failed to create socket, errorno : " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(9999); // htons : convert a number to network byte order

    bind_fd = bind(sock_fd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)); // <<<<< BIND
    if (bind_fd < 0)
    {
        std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
        exit (EXIT_FAILURE);
    }

    listen_fd = listen(sock_fd, 10);
    if (listen_fd < 0)
    {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit (EXIT_FAILURE);
    }
    
    auto addrlen = sizeof(sockaddr);

    connection = accept(sock_fd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen);
    if (connection < 0)
    {
        std::cout << "Failed to take a connection. errno: " << errno << std::endl;
        exit (EXIT_FAILURE);
    }

    while (1)
    {
        buff = (char *)malloc (sizeof(char) * 100);
        auto bytesRead = read(connection, buff, 100);
        std::cout << "The message was : " << buff;

        std::string response = "Good One \n";
        send(connection, response.c_str(), response.size(), 0);
        free (buff);
    }

    close (connection);
    close (sock_fd);

    return (0);
}