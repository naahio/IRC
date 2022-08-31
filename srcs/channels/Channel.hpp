/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 14:19:35 by hel-makh          #+#    #+#             */
/*   Updated: 2022/08/31 14:47:56 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>

# include "../tools/tool.hpp"

class User;

class Channel {
	private:
		std::string				name;
		std::string				topic;
		std::string				key;
		std::map <int, User *>	operators;
		std::map <int, User *>	members;

		Channel(void) {}
	
	public:
		Channel(std::string _name);
		~Channel(void) {}

		std::string const &			getName(void) const;
		std::string const &			getTopic(void) const;
		std::string const &			getKey(void) const;
		std::map <int, User *> &	getOperators(void);
		std::map <int, User *> &	getMembers(void);

		void	setName(std::string _name);
		void	setTopic(std::string _topic);
		void	setKey(std::string _topic);

		User *	getMember(int fd);

		void	addOperator(int fd);
		void	removeOperator(int fd);
		void	addMember(User * member, int fd, std::string key = "");
		void	removeMember(int fd);
};

#endif