/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 14:19:35 by hel-makh          #+#    #+#             */
/*   Updated: 2022/09/10 11:28:27 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include <sys/types.h>
# include <sys/socket.h>

# include "../users/User.hpp"
# include "../tools/tool.hpp"

class Channel {
	private:
		std::string				name;
		std::string				topic;
		std::string				key;
		size_t					membersLimit;
		bool					memberChatOnly;
		bool					inviteOnly;
		bool					moderated;
		bool					topicSettable;

		std::map <int, User *>	members;
		std::vector <int>		operators;
		std::vector <int>		moderators;
		std::vector <int>		invitees;

		Channel(void) {}
	
	public:
		Channel(std::string _name);
		~Channel(void) {}

		std::string const &			getName(void) const;
		std::string const &			getTopic(void) const;
		std::string const &			getKey(void) const;
		size_t						getMembersLimit(void) const;
		bool						isMemberChatOnly(void) const;
		bool						isInviteOnly(void) const;
		bool						isModerated(void) const;
		bool						isTopicSettable(void) const;
		
		std::map <int, User *> &	getMembers(void);
		std::vector <int> &			getOperators(void);
		std::vector <int> &			getModerators(void);
		std::vector <int> &			getInvitees(void);

		void	setTopic(std::string _topic, int fd);
		void	setKey(std::string _topic, int fd);
		void	setLimit(size_t limit, int fd);
		void	setMemberChatOnly(bool option, int fd);
		void	setInviteOnly(bool option, int fd);
		void	setModerated(bool option, int fd);
		void	setTopicSettable(bool option, int fd);
		
		User *	getMember(int fd);
		User *	getOperator(int fd);
		User *	getModerator(int fd);
		User *	getInvitee(int fd);

		void	addOperator(int fd);
		void	removeOperator(int fd);
		void	addModerator(int fd);
		void	removeModerator(int fd);
		void	addInvitee(int fd);
		void	removeInvitee(int fd);
		void	addMember(User * member, std::string key = "");
		void	removeMember(int fd);
		
		void	broadCastMessage(std::string & message, int fd = -1);
};

#endif