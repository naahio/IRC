/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:27 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/29 17:36:12 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <unistd.h>
# include <iostream>
# include <string>

# include "../channels/Channel.hpp"

class User
{
	private:
		int			fd;
		std::string	username;
		std::string	nickname;
		std::string	password;
		std::map <std::string, Channel *>	channels;

		User(void) {}

	public:
		User(int fd);
		~User(void);

		int					getFd(void) const;
		std::string const &	getUsername(void) const;
		std::string const &	getNickname(void) const;
		std::string const &	getPassword(void) const;
		std::map <std::string, Channel *> &
							getChannels(void);
		Channel *			getChannel(std::string name);

		void	setFd(int _fd);
		void	setUsername(std::string user_name);
		void	setNickname(std::string nick_name);
		void	setPassword(std::string pass_word);

		bool	isAuth(void);

		void	joinChannel(Channel & _channel);
		void	leaveChannel(std::string name);
		
		class notInChannel : public std::exception {
			char const *	what() const throw() {
				return "You are not on that channel.";
			}
		};
};

#endif