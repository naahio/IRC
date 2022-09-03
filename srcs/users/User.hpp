/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:27 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/02 15:48:01 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <unistd.h>
# include <string>
# include <map>

# include "../tools/tool.hpp"

class Channel;

class User
{
	private:
		int			fd;
		std::string	username;
		std::string	nickname;
		std::string	servName;
		std::string	hostName;
		std::string	fullName;
		std::string	password;
		bool		registered;
		bool		connected;
		std::map <std::string, Channel *>	channels;
		
		User(void) {}

	public:
		User(int fd);
		~User(void);

		int					getFd(void) const;
		std::string const &	getUsername(void) const;
		std::string const &	getNickname(void) const;
		std::string const &	getServerName(void) const;
		std::string const &	getHostName(void) const;
		std::string const &	getFullName(void) const;
		std::string	const & getPassword(void) const;

		std::map <std::string, Channel *> &
							getChannels(void);
		Channel *			getChannel(std::string name);

		void	setFd(int _fd);
		void	setUsername(std::string user_name);
		void	setNickname(std::string nick_name);
		void	setServerName(std::string servname);
		void	setHostName(std::string hostname);
		void	setFullName(std::string fullName);
		void	setPassword(std::string &password);
		void	setRegistered(void);
		void	setConnected(void);

		bool	isAuth(void);
		bool	isRegistered(void);
		bool	isConnected(void);

		void	joinChannel(Channel & channel, std::string name);
		void	leaveChannel(std::string name);
};

#endif