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
    this->end_Server       = FALSE;
    this->compress_array   = FALSE;
    this-> nfds            = TRUE;
    this->current_size     = FALSE;
}

Server::~Server()
{
    std::cout << "Server deleted " << std::endl;
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

// void        Server::set_fds(struct pollfd fds[POLL_SIZE])
// {
//     this->fds = fds;
// }

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

int     Server::get_close_conn()
{
    return (this->close_conn);
}

void    Server::set_close_conn(int close_con)
{
    this->close_conn = close_con;
}

int     Server::get_timeout()
{
    return (this->timeout);
}

void    Server::set_timeout(int timeout)
{
    this->timeout = timeout;
}

int     Server::get_nfds()
{
    return (this->nfds);
}

void    Server::set_nfds(int nfds)
{
    this->nfds = nfds;
}

int     Server::get_current_size()
{
    return (this->current_size);
}

void    Server::set_current_size(int size)
{
    this->current_size = size;
}

char*   Server::get_buffer()
{
    return (this->buffer);
}

// void    Server::set_buffer(char buffer[BUFF_SIZE])
// {
//     this->buffer = buffer;
// }

int    Server::Creat_socket()
{
    this->socket_fd = socket(AF_INET6, SOCK_STREAM, 0);

    if (this->socket_fd < 0)
    {
        std::cout << "Failed to create socket, errno : " << errno << std::endl;
        return (0);
    }
    std::cout << "Socket Created successfully ." << std::endl;
    return (1);
}

int     Server::reusable_socket()
{
    this->rc = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&this->on, sizeof(this->on));

    if (this->rc < 0)
    {
        std::cout << "Failed at setting socket option ! errno : " << errno << std::endl;
        return (0); 
    }
    std::cout << "setting socket option . . . " << std::endl;
    std::cout << "Done ! " << std::endl;
    return (1);
}

int     Server::nonblocking_socket()
{
    this->rc = ioctl(this->socket_fd, FIONBIO, (char *)&this->on);

    if (this ->rc < 0)
    {
        std::cout << "Failed at making the socket non_blocking ! errno : " << errno << std::endl;
        return (0);
    }
    std::cout << "making socket non blocking . . . " << std::endl;
    std::cout << "Done !" << std::endl;
    return (1);
}


int     Server::bind_socket()
{
    memset(&this->addr, 0, sizeof(this->addr));
    this->addr.sin6_family = AF_INET6;
    memcpy(&this->addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    addr.sin6_port = htons(SER_PORT);
    this->rc = bind(this->socket_fd, (struct sockaddr *)&this->addr, sizeof(addr));
    if (this->rc < 0)
    {
        std::cout << "Failed at binding the socket ! errno : " <<  errno << std::endl;
        return (0);
    }
    std::cout << "getting PORT and IP . . . " << std::endl;
    std::cout << "Done !" << std::endl;
    return (1);
}

int     Server::listen_from_socket()
{
    this->rc = listen(this->socket_fd, MAX_CANON);

    if (this->rc < 0)
    {
        std::cout << "Failed at listing ! errno : " << errno << std::endl;
        return (0);
    }
    std::cout << "listing . . ." << std::endl;
    return (1);
}

void    Server::poll_trait()
{
    std::cout << "setting poll structure . . ." << std::endl;
    memset(this->fds, 0, sizeof(this->fds));

    this->fds[0].fd = this->socket_fd;
    this->fds[0].events = POLLIN;

    std::cout << "Done !" << std::endl;
}

void     Server::accept_connect()
{
    std::cout << "Waiting for connection . . . " << std::endl;
    do
    {
        this->new_fd = accept(this->socket_fd, NULL, NULL);
        if (this->new_fd < 0)
        {
            if (errno != EWOULDBLOCK)
            {
                std::cout << "FAILED at accepting connection ! errno : " << errno << std::endl;
                this->end_Server = TRUE; 
            }
            break;
        }
        std::cout << "NEW Connection detected "<< this->new_fd << std::endl;
        this->fds[this->nfds].fd = this->new_fd;
        this->fds[this->nfds].events = POLLIN;
        this->nfds++;
    }while (this->new_fd != -1);
}

void        Server::recv_send_msg(int i)
{
    std::cout <<  "receiving . . ." << std::endl;
    this->close_conn = FALSE;
    do
    {
        this->rc = recv(this->fds[i].fd, this->buffer, sizeof(this->buffer), 0);
        std::cout << this->buffer << std::endl;s
        if (this->rc < 0)
        {
            if (errno != EWOULDBLOCK)
            {
                std::cout << "FAILED at receiving a msg ! errno : " << errno << std::endl;
                this->close_conn = TRUE;
            }
            break;
        }
        if (this->rc == 0)
        {
            std::cout << "Connection closed . . . " << std::endl;
            this->close_conn = TRUE;
            break;
        }
        this->len = this->rc;
        std::cout << len << " bytes received " << std::endl;
        // this->buffer[this->len] = '\0';
        this->rc = send(this->fds[i].fd, this->buffer, this->rc, 0);
        if (this->rc < 0)
        {
            std::cout << "FAILED to send an answer to the client ! " << std::endl;
            this->close_conn = TRUE;
            break;
        }
        this->rc = send(this->fds[i].fd, "received succ >.<\n", sizeof("received succ >.<\n"), 0);
        if (this->rc < 0)
        {
            std::cout << "FAILED to send an answer to the client ! " << std::endl;
            this->close_conn = TRUE;
            break;
        }
    }while (TRUE);
}