/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 10:07:44 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/27 16:48:16 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Player.hpp"

Player::Player()
{
}

Player::Player(User *user)
{
	this->user		= user;
	this->nick		= user->getNickname();
	this->level		= "LEVEL_0";
	this->status	= "normal";
	this->logtime	= 0;
	this->rank		= "newbie";
	this->points	= 0;
}

Player::Player(int fd, std::string nick, std::string post, std::string lvl, std::string status, int logtime, std::string grad, int points)
{
	this->user		= NULL;
	this->fd		= fd;
	this->nick		= nick;
	this->post 		= post;
	this->level		= lvl;
	this->status	= status;
	this->logtime	= logtime;
	this->rank		= grad;
	this->points	= points;
}

Player::~Player()
{
}

std::string	Player::getnickname()
{
	return (this->nick);
}

User *	Player::getUser()
{
	return (this->user);
}

std::string	Player::getLevel()
{
	return (this->level);
}

std::string	Player::getStatus()
{
	return (this->status);
}

int	Player::getLogtime()
{
	return (this->logtime);
}

std::string	Player::getRank()
{
	return (this->rank);
}

int		Player::getPoint()
{
	return (this->points);
}

std::string	Player::getPost()
{
	return (this->post);
}

int		Player::getLoged_In()
{
	return (this->loged_in);
}	

int		Player::getFD()
{
	return (this->fd);
}

void	Player::set_Loged_In(int time)
{
	this->loged_in = time;
}

Player& Player::operator = (const Player& player)
{
	this->nick		= player.nick;
	this->level		= player.level;
	this->status	= player.status;
	this->rank		= player.rank;
	this->logtime 	= player.logtime;
	return (*this);
}

void	Player::set_user(User *user)
{
	this->user = user;
}

void	Player::set_logtime(int time)
{
	this->logtime = time;
}

void	Player::add_Points(int point)
{
	this->points += point;
}

void	Player::Level_Up()
{
	std::string current_lvl = this->level;
	
	if (this->points >= LVL10)
		this->level = ("LEVEL_10");
	else if (this->points >= LVL9)
		this->level = ("LEVEL_9");
	else if (this->points >= LVL8)
		this->level = ("LEVEL_8");
	else if (this->points >= LVL7)
		this->level = ("LEVEL_7");
	else if (this->points >= LVL6)
		this->level = ("LEVEL_6");
	else if (this->points >= LVL5)
		this->level = ("LEVEL_5");
	else if (this->points >= LVL4)
		this->level = ("LEVEL_4");
	else if (this->points >= LVL3)
		this->level = ("LEVEL_3");
	else if (this->points >= LVL2)
		this->level = ("LEVEL_2");
	else if (this->points >= LVL1)
		this->level = ("LEVEL_1");
	else if (this->points >= LVL0)
		this->level = ("LEVEL_0");
	if (this->level.compare(current_lvl))
		if (this->user)
		{
			sendReply(this->user->getFd(), ":irc!~irc1337 NOTICE HH : ⚔️  LEVEL UP ! ⚔️ \n");
			sendReply(this->user->getFd(), stringBuilder(3, ":irc!~irc1337 NOTICE HH : ⚔️  NEW RANK ", this->level.c_str() , " ⚔️ \n"));
			this->Promote();
			this->Update_Status();
			// this->points = 0;
		}
}

void	Player::Update_Status()
{
	if (this->points > LVL2)
	{
		if (this->logtime > this->points)
			this->status = "LogTime_Seeker";
		else if (this->points > this->logtime * 2)
			this->status = "Fighter";
		else if (this->points > this->logtime )
			this->status = "Normal_Player";
	}
}

void		Player::Promote()
{
	if (!this->level.compare("LEVEL_2"))
		this->rank = "user";
	else if (!this->level.compare("LEVEL_4"))
		this->rank = "player";
	else if (!this->level.compare("LEVEL_6"))
		this->rank = "CREATOR";
	else if (!this->level.compare("LEVEL_8"))
		this->rank = "OPERATOR";
	else if (!this->level.compare("LEVEL_10"))
		this->rank = "ADMIN";
}
