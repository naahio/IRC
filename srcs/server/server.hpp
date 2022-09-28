/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 13:13:06 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/28 11:14:58 by hel-makh         ###   ########.fr       */
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
# include <fstream>
# include <sstream>
# include <ctime>
# include <sys/types.h>
# include <sys/stat.h>
# include <netdb.h>

# include "../channels/Channel.hpp"
# include "../tools/tool.hpp"
# include "../pars/msg.hpp"
# include "../bot/player/Player.hpp"

# define BUFF_SIZE		512
# define MAX_CONN		64
# define TIMEOUT		30 * 60 * 1000

class Server
{
	private:
		std::map <int, User *>				users;
		std::map <int, User *>				guests; 
		std::map <std::string, Channel *>	channels;
		std::map <std::string, std::string> operators;
		
		std::map <std::string, Player *> players_list;


		int				socket_fd;
		struct pollfd	fds[MAX_CONN + 1];
		int				nfds;
		int				on;
		int				port;
		std::string		password;
		std::string		name;
		std::string		version;
   		char 			*creationTime;

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
		std::map <int, User *> &			        getGuests(void); 
		std::map <std::string, Channel *> &	        getChannels(void);
		std::map <std::string, std::string> &  		getOperators(void);

		std::map <std::string, Player *> &	        getPlayers_List(void);
	
		User	*	getUser(int fd);
		User	*	getUser(std::string nickname);
		User	*	getGuest(int fd);
		User	*	getGuest(std::string nickname);
		Channel	*	getChannel(std::string name);

		void	addUser(int fd,User *user);
		void	addGuest(int fd,char *ip, char *postname);  
		void	clientDisconnect(int fd);
		void	listUserModes(User * user, int fd);

		void	createChannel(std::string name, User & op);
		void	deleteChannel(std::string name);
		void	listChannelModes(Channel * channel, int fd);
		void	listChannelBans(Channel * channel, int fd);

		void	parsExecCommands(Msg &msg);
		void	cmdExec(Msg &msg,std::vector<std::string> &cmd);
		int		splitCmd(std::string &cmd,
						std::vector<std::string> &oneCmdParsed);

		bool	ctcpMessage(std::string &cmd,std::vector<std::string> &vec);
		void	fileTransfer(int fd,std::string & nick,std::vector<std::string> &vec);

		int		Create_socket(void);
		int		reusable_socket(void);
		int		nonblocking_socket(void);
		int		bind_socket(void);
		int		listen_from_socket(void);
		void	poll_trait(void);
		bool	accept_connections(void);
		bool	recv_send_msg(int fd);

		//**************** Commands : 

		void	PASScmd(int		fd, std::vector<std::string> &cmd);
		void	NICKcmd(int		fd, std::vector<std::string> &cmd);
		void	USERcmd(int		fd, std::vector<std::string> &cmd);
		void	OPERcmd(int		fd, std::vector<std::string> &cmd);
		void	PRIVMSGcmd(int	fd, std::vector<std::string> &cmd, bool notice = false);
		void	LISTcmd(int		fd, std::vector<std::string> &cmd);
		void	NAMEScmd(int	fd, std::vector<std::string> &cmd);
		void	JOINcmd(int		fd, std::vector<std::string> &cmd);
		void	PARTcmd(int		fd, std::vector<std::string> &cmd);
		void	MODEcmd(int		fd, std::vector<std::string> &cmd);
		void	TOPICcmd(int	fd, std::vector<std::string> &cmd);
		void	INVITEcmd(int	fd, std::vector<std::string> &cmd);
		void	KICKcmd(int		fd, std::vector<std::string> &cmd);
		void	KILLcmd(int		fd, std::vector<std::string> &cmd);
		void    HELPcmd(int		fd);
		void	VERSIONcmd(int	fd);
		void	TIMEcmd(int		fd);
		void	ADMINcmd(int	fd);
		void	QUITcmd(int		fd, std::vector<std::string> &cmd);
		void	SENDcmd(int		fd, std::vector<std::string> &cmd);
		void	RESPONDcmd(int	fd, std::vector<std::string> &cmd);
		
		void	sendingFile(User *sender,User *reciever,size_t fileSize);

		void	sendChannelUsers(int fd, Channel *chan,User *user,const std::string & channel);
		void	welcomeReply(int fd);
		void	channelModes(int fd, std::vector<std::string> & cmd);
		void	userModes(int fd, std::vector<std::string> & cmd);

		void 	DataToFile();
		void	add_player(User *user);
		void	link_data(User *user);
		bool	load_data();
		void	save_data();
		bool	check_exist(User *user);

		Player *	getPlayer(std::string nickname);
		Player *	getPlayer(int fd);
		
};

#endif
