/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/27 14:19:35 by hel-makh          #+#    #+#             */
/*   Updated: 2022/08/29 17:35:53 by hel-makh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <map>

class User;

class Channel {
	private:
		std::string						name;
		User *							op;
		std::map <std::string, User *>	members;

		Channel(void) {}
	
	public:
		Channel(std::string _name, User & _operator);
		~Channel(void) {}

		std::string const &					getName(void) const;
		User *								getOperator(void) const;
		std::map <std::string, User *> &	getMembers(void);

		void	setName(std::string _name);
		void	setOperator(User * _operator);

		void	addMember(User * member, std::string username);
		void	removeMember(std::string username);
};

#endif