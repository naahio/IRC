/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 13:13:06 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/19 10:43:08 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/poll.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <netinet/in.h>
# include <errno.h>
# include <iostream>
# include <unistd.h>
# include <iterator>
# include <map>

# include "../users/User.hpp"


# define SER_PORT 9999

# define TRUE           1
# define FALSE          0
# define F_INIT         -1
# define BUFF_SIZE      100
# define POLL_SIZE      200
# define MAX_CONN       32

class Server
{
    private:
        std::map <int, User>    users;
        struct sockaddr_in6     addr;
        struct pollfd           fds[POLL_SIZE];
        int                     len;
        int                     rc;
        int                     on;
        int                     socket_fd;
        int                     new_fd;
        int                     desc_ready;
        int                     end_Server;
        int                     compress_array;
        int                     close_conn;
        char                    buffer[BUFF_SIZE];
        int                     timeout;
        int                     nfds;
        int                     current_size;
        int                     i;
        int                     j;

    public:
        Server();
        ~Server();

        std::map <int, User>    get_users();
        void                    set_users(std::map <int, User> u_map);
        struct sockaddr_in6     get_addr();
        void                    set_addr(struct sockaddr_in6     addr);
        struct pollfd*          get_fds();
        void                    set_fds(struct pollfd fds[POLL_SIZE]);
        int                     get_len();              
        void                    set_len(int len);              
        int                     get_rc();
        void                    set_rc(int rc);
        int                     get_socket_fd();
        void                    set_socket_fd(int socket_fd);
        int                     get_new_fd();
        void                    set_new_fd(int new_fd);
        int                     get_desc_ready();
        void                    set_desc_ready(int fd_ready);
        int                     get_end_Server();
        void                    set_end_Server(int end_serv);
        int                     get_compress_array();
        void                    set_compress_array(int comp_arr);
        int                     get_close_conn();
        void                    set_close_conn(int close_con);
        int                     get_timeout();
        void                    set_timeout(int timeout);
        int                     get_nfds();
        void                    set_nfds(int nfds);
        int                     get_current_size();
        void                    set_current_size(int size);
        int                     get_i();
        void                    set_i(int i);
        int                     get_j();
        void                    set_j(int j);
        char*                   get_buffer();
        void                    set_buffer(char buffer[BUFF_SIZE]);

        void    Creat_socket();
        void    reusable_socket();
        void    nonblocking_socket();
        void    bind_socket();
        void    listen_from_socket();
        void    poll_trait();
        void    accept_connect();
        void    recv_msg();
        void    send_msg();


};


#endif