/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:27 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/18 11:58:07 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <string>

class User
{
    private:
        int         fd;
        std::string username;
        std::string nickname;
        std::string password;
        bool        is_name;
        bool        is_nick;
        bool        is_pass;

    public:
        User();
        User(fd);
        ~User();
        bool        is_Auth()
        std::string get_username() const;
        std::string get_nickname() const;
        std::string get_password() const;

        void    set_username();
        void    set_nickname();
        void    set_password();
};




#endif