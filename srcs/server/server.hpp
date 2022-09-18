/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 13:13:06 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/18 17:07:10 by ybensell         ###   ########.fr       */
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
# include <cstring>
# include <iterator>
# include <map>
# include <fcntl.h>
# include <arpa/inet.h>
# include <sstream>
# include <ctime>

# include "../users/User.hpp"
# include "../channels/Channel.hpp"
# include "../tools/tool.hpp"
# include "../pars/msg.hpp"
# include "../tools/Commands.hpp"

# define BUFF_SIZE		1024
# define MAX_CONN		50
# define TIMEOUT		3 * 60 * 1000
# define SERVNAME 		

class Server
{
	private:
		std::map <int, User *>				users;
		std::map <std::string, Channel *>	channels;
		std::map <std::string, std::string> operators;
		
		int				socket_fd;
		struct pollfd	fds[MAX_CONN];
		int				nfds;
		int				on;
		int				port;
		std::string		password;
		std::string		name;
		std::string		version;

		Server(void) {}

	public:
		Server(int port, std::string password);
		~Server(void);

		int					getSocketFd(void) const;
		struct pollfd *		getFds(void);
		int					getNfds(void) const;
		void				setNfds(int nfds);
		int					getPort(void) const;
		std::string const &	getPass(void) const;
		std::string const &	getName(void) const;
		std::string const & getVersion(void) const;

		std::map <int, User *> &			        getUsers(void);
		std::map <std::string, Channel *> &	        getChannels(void);
		std::map <std::string, std::string> &  		getOperators(void);
	
		User	*	getUser(int fd);
		User	*	getUser(std::string nickname);
		Channel	*	getChannel(std::string name);

		void	addUser(int fd,char *ip);
		void	clientDisconnect(int fd);

		void	createChannel(std::string name, User & op);
		void	deleteChannel(std::string name);
		
		void	parsExecCommands(Msg &msg);
		void	cmdExec(Msg &msg,std::vector<std::string> &cmd);
		void	splitCmd(std::string &cmd,
						std::vector<std::string> &oneCmdParsed);

		int		Create_socket(void);
		int		reusable_socket(void);
		int		nonblocking_socket(void);
		int		bind_socket(void);
		int		listen_from_socket(void);
		void	poll_trait(void);
		bool	accept_connections(void);
		bool	recv_send_msg(int fd);

		//**************** Commands : 

		void	USERcmd(int		fd,  std::vector<std::string> &cmd);
		void	NICKcmd(int		fd,	std::vector<std::string> &cmd);
		void	PASScmd(int		fd,	std::vector<std::string> &cmd);
		void	JOINcmd(int		fd,	std::vector<std::string> &cmd);
		void	PRIVMSGcmd(int	fd,	std::vector<std::string> &cmd);
		void	INVITcmd(int	fd,	std::vector<std::string> &cmd);
		void	QUITcmd(int		fd,	std::vector<std::string> &cmd);
		void	OPERcmd(int		fd,	std::vector<std::string> &cmd);
		void	KILLcmd(int		fd,	std::vector<std::string> &cmd);
		void	VERSIONcmd(int	fd);
		void	TIMEcmd(int		fd);
		void	ADMINcmd(int	fd);

		void    kick(int fd, std::vector<std::string> &cmd);
		void    helps(int fd);
		void    part(int fd, std::vector<std::string> &cmd);
		void	list(int fd, std::vector<std::string> &cmd);
		void    mode(int fd, std::vector<std::string> &cmd);
		void    names(int fd, std::vector<std::string> &cmd);
		void    topic(int fd, std::vector<std::string> &cmd);

		void	sendChannelUsers(int fd, Channel *chan,User *user,const std::string & channel);
		void	channelModes(int fd, std::vector<std::string> & cmd);
		void	userModes(int fd, std::vector<std::string> & cmd);
};

#endif