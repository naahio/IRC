/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serv.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 13:13:06 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/18 13:20:29 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERV_HPP
# define SERV_HPP

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


# define SERVER_PORT 9999

# define TRUE           1
# define FALSE          0
# define F_INIT         -1
# define BUFF_SIZE      100
# define POLL_SIZE      200
# define MAX_CONN       32

class Serv
{
    private:

    public:
        std::map <int, User> users;
        Serv();
        ~Serv();
        
        
};


#endif