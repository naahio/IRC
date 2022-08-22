/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:27 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/22 12:35:59 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <string>

class User
{
    private:
        // int         fd;
        std::string username;
        std::string nickname; //unique
        std::string password;
        bool        is_name;
        bool        is_nick;
        bool        is_pass;
        int         _IP;

    public:
        User();
        User(int fd);
        ~User();
        std::string get_username() ;
        std::string get_nickname() ;
        std::string get_password() ;

        void    set_username(std::string user_name);
        void    set_nickname(std::string nick_name);
        void    set_password(std::string pass_word);
        

        bool        check_auth();
};




#endif