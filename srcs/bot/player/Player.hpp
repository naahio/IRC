/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 09:11:58 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/27 08:56:36 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_HPP
# define PLAYER_HPP

# include <map>
# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <cstring>
# include <iterator>
# include <fstream>
# include <sstream>

# include "../../users/User.hpp"

# define CREATED_CHANNEL_POINT	5
# define CHANNEL_POINT 			3
# define COMMANDS_POINT			1

# define LVL0					0
# define LVL1					50
# define LVL2					150
# define LVL3					200
# define LVL4					250
# define LVL5					260
# define LVL6					340
# define LVL7					440
# define LVL8					640
# define LVL9					790
# define LVL10					10000

class Player
{
	private:
		User*		user;
		int			fd;
		std::string	nick;
		std::string	post;
		std::string	level;
		std::string	status;
		int			logtime;
		std::string	rank;
		int			points;
		int			loged_in;

	public:
		Player();
		Player(User *user);
		Player(int fd, std::string nick, std::string post, std::string lvl, std::string status, int logtime, std::string grad, int point);
		~Player();

		User *		getUser();
		int 		getFD();
		std::string	getnickname();
		std::string getPost();
		std::string	getLevel();
		std::string	getStatus();
		int			getLogtime();
		std::string	getRank();
		int			getPoint();
		int			getLoged_In();

		
		void		set_user(User *user);
		void		set_logtime(int time);
		void		set_Loged_In(int time);


		void		Level_Up();
		void		Update_Status();
		void		Promote();
		void		add_Points(int point);



		Player&		operator = (const Player& player);
};


#endif