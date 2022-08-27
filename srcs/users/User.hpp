/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:27 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/27 17:17:29 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <unistd.h>
# include <iostream>
# include <string>

class User
{
	private:
		int			fd;
		std::string	username;
		std::string	nickname;
		std::string	password;
		bool		is_name;
		bool		is_nick;
		bool		is_pass;

		User(void) {};

	public:
		User(int fd);
		~User(void);

		int					getFd(void) const;
		std::string const &	getUsername(void) const;
		std::string const &	getNickname(void) const;
		std::string const &	getPassword(void) const;

		void	setUsername(std::string user_name);
		void	setNickname(std::string nick_name);
		void	setPassword(std::string pass_word);

		bool	isAuth(void);
};

#endif