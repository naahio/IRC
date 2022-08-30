/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:27 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/30 16:13:02 by ybensell         ###   ########.fr       */
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
		std::string	servername;
		std::string	hostname;
		std::string	fullName;
		std::map <std::string, Channel *>	channels;
		
		User(void) {}

	public:
		User(int fd);
		~User(void);

		int					getFd(void) const;
		std::string const &	getUsername(void) const;
		std::string const &	getNickname(void) const;
		std::string const &	getPassword(void) const;
		std::string const &	getServerName(void) const;
		std::string const &	getHostName(void) const;
		std::string const &	getFullName(void) const;
		std::map <std::string, Channel *> &
							getChannels(void);
		Channel *			getChannel(std::string name);
		
		void	setFd(int _fd);
		void	setUsername(std::string user_name);
		void	setNickname(std::string nick_name);
		void	setPassword(std::string pass_word);
		void	setServerName(std::string servername);
		void	setHostName(std::string hostname);
		void	setFullName(std::string fullName);
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