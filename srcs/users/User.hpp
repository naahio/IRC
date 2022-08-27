/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:27 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/23 15:22:26 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <string>

class User
{
	private:
		int			fd;
		std::string	ip;
		std::string	username;
		std::string	nickname;
		std::string	password;
		bool		is_name;
		bool		is_nick;
		bool		is_pass;

		User(void) {};

	public:
		User(std::string ip, int fd);
		~User(void);

		int					get_fd(void) const;
		std::string const &	get_ip(void) const;
		std::string const &	get_username(void) const;
		std::string const &	get_nickname(void) const;
		std::string const &	get_password(void) const;

		void	set_username(std::string user_name);
		void	set_nickname(std::string nick_name);
		void	set_password(std::string pass_word);

		bool	check_auth();
};

#endif