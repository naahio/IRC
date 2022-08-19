/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 10:53:11 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/19 10:44:25 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "server.hpp"


Server::Server()
{
    std::cout << "Creating Serverer . . . " << std::endl;
    std::cout << "updating parameter . . . " << std::endl;

    this->len              = TRUE;
    this->rc               = TRUE;
    this->on               = TRUE;
    this->socket_fd        = F_INIT;
    this->new_fd           = F_INIT;
    this->end_Serverer       = FALSE;
    this->compress_array   = FALSE;
    this-> nfds            = TRUE;
    this->current_size     = FALSE;
}

Server::~Server()
{
    std::cout << "Serverer deleted " << std::endl;
}

std::map<int, User> Server::get_users()
{
    return (this->users);
}

void    Server::set_users(std::map <int, User> u_map)
{
    this->users = u_map;
}

struct sockaddr_in6     Server::get_addr()
{
    return (this->addr);
}

void    Server::set_addr(struct sockaddr_in6     addr)
{
    this->addr = addr;
}

struct pollfd*        Server::get_fds()
{
    return (this->fds);
}

void        Server::set_fds(struct pollfd fds[POLL_SIZE])
{
    this->fds = fds;
}

int Server::get_len()
{
    return (this->len);
}

void    Server::set_len(int len)
{
    this->len = len;
}

int Server::get_rc()
{
    return (this->rc);
}

void    Server::set_rc(int rc)
{
    this->rc = rc;
}

int     Server::get_socket_fd()
{
    return (this->socket_fd);
}

void    Server::set_socket_fd(int socket_fd)
{
    this->socket_fd = socket_fd;
}

int     Server::get_new_fd()
{
    return (this->new_fd);
}

void    Server::set_new_fd(int new_fd)
{
    this->new_fd = new_fd;
}

int     Server::get_desc_ready()
{
    return (this->desc_ready); 
}

void    Server::set_desc_ready(int fd_ready)
{
    this->desc_ready = fd_ready;
}

int     Server::get_end_Server()
{
    return (this->end_Server);
}

void    Server::set_end_Server(int end_serv)
{
    this->end_Server = end_serv;
}

int     Server::get_compress_array()
{
    return (this->compress_array);
}

void    Server::set_compress_array(int comp_arr)
{
    this->compress_array = comp_arr;
}



