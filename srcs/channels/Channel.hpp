/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 14:19:35 by hel-makh          #+#    #+#             */
/*   Updated: 2022/09/27 16:34:13 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include <ctime>
# include <sys/types.h>
# include <sys/socket.h>
# include <string.h>
# include <iostream>

# include "../users/User.hpp"
# include "../tools/tool.hpp"

typedef struct s_bans {
	std::string	banMask;
	std::string	banMod;
	std::time_t	banTimestamp;
}	t_bans;

class Channel {
	private:
		std::string				name;
		std::string				topic;
		std::string				key;
		std::time_t				creationTimestamp;
		size_t					membersLimit;
		bool					_private;
		bool					secret;
		bool					memberChatOnly;
		bool					inviteOnly;
		bool					moderated;
		bool					topicSettableByOp;

		std::map <int, User *>	members;
		std::map <int, User *>	invitees;
		std::vector <int>		operators;
		std::vector <int>		moderators;
		std::vector <t_bans>	bans;

		Channel(void) {}
	
	public:
		Channel(std::string _name);
		~Channel(void) {}

		std::string const &			getName(void) const;
		std::string const &			getTopic(void) const;
		std::string const &			getKey(void) const;
		std::time_t					getCreationTimestamp(void) const;
		size_t						getMembersLimit(void) const;
		bool						isPrivate(void) const;
		bool						isSecret(void) const;
		bool						isMemberChatOnly(void) const;
		bool						isInviteOnly(void) const;
		bool						isModerated(void) const;
		bool						isTopicSettableByOp(void) const;
		
		std::map <int, User *> &	getMembers(void);
		std::map <int, User *> &	getInvitees(void);
		std::vector <int> &			getOperators(void);
		std::vector <int> &			getModerators(void);
		std::vector <t_bans> &		getBans(void);

		void	setTopic(std::string _topic, int fd);
		void	setKey(std::string _key, bool option, int fd);
		void	setLimit(size_t limit, int fd);
		void	setPrivate(bool option, int fd);
		void	setSecret(bool option, int fd);
		void	setMemberChatOnly(bool option, int fd);
		void	setInviteOnly(bool option, int fd);
		void	setModerated(bool option, int fd);
		void	setTopicSettableByOp(bool option, int fd);
		
		User *	getMember(int fd);
		User *	getInvitee(int fd);
		User *	getOperator(int fd);
		User *	getModerator(int fd);

		void	addMember(User * member, std::string _key = "");
		void	removeMember(int fd);
		void	addInvitee(User * invitee);
		void	removeInvitee(int fd);
		void	addOperator(int fd, int opFd = -1);
		void	removeOperator(int fd, int opFd = -1);
		void	addModerator(int fd, int opFd = -1);
		void	removeModerator(int fd, int opFd = -1);
		void	addBan(std::string const & banMask, int fd);
		void	removeBan(std::string const & banMask, int fd);

		bool	isBanned(User * user);
		bool	isBanned(std::string banMask);
		
		void	broadCastMessage(std::string const & message, int fd = -1, bool everyone = true);
};

#endif