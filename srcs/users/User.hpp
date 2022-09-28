/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 11:25:27 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/28 09:51:35 by hel-makh         ###   ########.fr       */
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
		std::string	msgRemainder;
		std::string	ipAddress;
		std::string	postnumber;
		std::string	log;
		bool	registered;
		bool	connected;
		bool	visible;
		bool    isoperator;
		
		std::map <std::string, Channel *>	channels;
		std::map<std::string,size_t>  files;
		User(void) {}

	public:
		User(int fd,char *ip, std::string postname);
		~User(void);

		int					getFd(void) const;
		std::string const &	getUsername(void) const;
		std::string const &	getNickname(void) const;
		std::string const &	getServerName(void) const;
		std::string const &	getHostName(void) const;
		std::string const &	getFullName(void) const;
		std::string	const & getPassword(void) const;
		std::string const & getMsgRemainder(void) const;
		std::string const & getIpAddress(void) const;
		std::string const	getIdentifier(void) const;
		std::string const & getPostNumber(void) const;
		std::string const & getLog(void) const;
		std::map <std::string, Channel *> &
							getChannels(void);
		Channel *			getChannel(std::string name);
		std::map<std::string,size_t> &
							getFiles(void) ;

		void	setFd(int _fd);
		void	setUsername(std::string user_name);
		void	setNickname(std::string nick_name);
		void	setServerName(std::string servname);
		void	setHostName(std::string hostname);
		void	setFullName(std::string fullName);
		void	setPassword(std::string &password);
		void	setMsgRemainder(std::string &remaining);
		void	setRegistered(void);
		void	setConnected(void);
		void	setVisibility(bool option);
		void	setIsOperator(void);
		void	setFiles(std::string filename,size_t filesize);
		void	setLog(std::string time);
		bool	isAuth(void);
		bool	isRegistered(void);
		bool	isConnected(void);
		bool	isVisible(void);
		bool	isOperator(void);

		void	removeFile(std::string file);
		void	joinChannel(Channel & channel, std::string name);
		void	leaveChannel(std::string name);

		User*	operator = (const User& user);
};

#endif