/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 09:11:58 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/25 13:40:19 by mbabela          ###   ########.fr       */
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

# define LVL1					50
# define LVL2					150
# define LVL3					200
# define LVL4					250
# define LVL5					260
# define LVL6					340
# define LVL7					440
# define LVL8					640
# define LVL9					790
# define LVL10					1000

class Player
{
	private:
		User*		user;
		std::string	nick;
		std::string	level;
		std::string	status;
		int			logtime;
		std::string	grad;
		int			points;
		int			loged_in;

	public:
		Player();
		Player(User *user);
		Player(std::string nick, std::string lvl, std::string status, int logtime, std::string grad, int point);
		~Player();

		User *								getUser();
		std::string							getnickname();
		std::string							getLevel();
		std::string							getStatus();
		int									getLogtime();
		std::string							getGrad();
		int									getPoint();
		int									getLoged_In();
		// void								getlog_out(User *user, std::string out_time);


		Player *							getPlayer(std::map<std::string, Player *> players, std::string nickname);
		Player *							getPlayer(std::map<std::string, Player *> players_list, int fd);
		
		void								set_user(User *user);
		void								set_logtime(int time);
		void								set_Loged_In(int time);

		void								add_player(std::map<std::string, Player *> &players_list, User *user);
		void								link_data(std::map<std::string, Player *> players_list, std::map<std::string, Player *> players, User *user);
		bool								load_data(std::map<std::string, Player *> &players);
		void								save_data(std::map<std::string, Player *> players_list);
		bool								check_exist(std::map<std::string, Player *> players, User *user);
		
		std::string							Level_Up(int points);
		std::string							Update_Status();
		std::string							Promoted(std::string level);
		std::string							Calc_LogTime();
		void								add_Points(int point);

		Player&								operator = (const Player& player);
};


#endif