/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 10:07:44 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/25 14:43:47 by mbabela          ###   ########.fr       */
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
	this->level		= "0";
	this->status	= "newbie";
	this->logtime	= 0;
	this->grad		= "newbie";
	this->points	= 0;
}

Player::Player(std::string nick, std::string lvl, std::string status, int logtime, std::string grad, int points)
{
	this->nick		= nick;
	this->level		= lvl;
	this->status	= status;
	this->logtime	= logtime;
	this->grad		= grad;
	this->points	= points;
}

Player::~Player()
{
}

// std::map<std::string, Player *> & Player::get_playerslist()
// {
// 	return (this->players_list);
// }

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

std::string	Player::getGrad()
{
	return (this->grad);
}

int		Player::getPoint()
{
	return (this->points);
}

int		Player::getLoged_In()
{
	return (this->loged_in);
}	

void	Player::set_Loged_In(int time)
{
	this->loged_in = time;
}

// void	Player::getlog_out(User *user, std::string out_time)
// {
// 	std::string time;

// 	(void)out_time;
// 	time = user->getLog();//- out_time;
// 	time = "125h";
// 	this->set_logtime(time);
// }

Player& Player::operator = (const Player& player)
{
	this->nick		= player.nick;
	this->level		= player.level;
	this->status	= player.status;
	this->grad		= player.grad;

	return (*this);
}


Player *     Player::getPlayer(std::map<std::string, Player *> players, std::string nickname)
{
	std::map<std::string, Player *>::iterator	it;

	for (it = players.begin(); it != players.end(); ++it) 
	{
		Player *player = it->second;
		if (player->getnickname() == nickname)
			return (player);
	}
	return (NULL);
}

Player *     Player::getPlayer(std::map<std::string, Player *> players_list, int fd)
{
	std::map<std::string, Player *>::iterator	it;

	for (it = players_list.begin(); it != players_list.end(); ++it) 
	{
		if (it->second->getUser()->getFd() == fd)
			return (it->second);
	}
	return (NULL);
}

void	Player::set_user(User *user)
{
	this->user = user;
}

void	Player::set_logtime(int time)
{
	this->logtime = time;
}

void	Player::add_player(std::map<std::string, Player *> &players_list, User *user) // first time connected
{
	time_t now = time(0);
	Player *player = new Player(user);
	player->set_Loged_In(now);
	std::cout << "NEW PLAYER ADDED  : " << player->nick << std::endl;
	players_list.insert(std::pair<std::string, Player *>(user->getNickname(), player));
	// this->save_data(players_list);
}

void	Player::link_data(std::map<std::string, Player *> players_list, std::map<std::string, Player *> players, User *user)
{
	std::cout << " geting player  " << std::endl;
	Player *player = getPlayer(players, user->getNickname());
	player->set_user(user);
	time_t now = time(0);
	player->set_Loged_In(now);
	std::cout << " PLAYER LINKED  : " << player->nick << std::endl;
	players_list.insert(std::pair<std::string, Player *>(user->getNickname(), player));
	std::cout << " SAVED " << std::endl;
}

bool	Player::load_data(std::map<std::string, Player *> &players)
{
	std::ifstream	file;
	std::string		data;
	std::vector<std::string> p_data;

	std::cout << " Loading data  " << std::endl;
	file.open("/Users/mbabela/Desktop/IRC/user.txt");
	if (!file)
	{
		std::cout << "Error ! could not open the file " <<std::endl;
		return (false);
	}
	while (getline (file, data))
	{
		std::cout << "files lanes : " << data << std::endl;
		split(data, ' ', p_data);
		std::cout << "found  : " << p_data[0] << std::endl;
		Player *player = new Player(p_data[0], p_data[1], p_data[2], ft_toInt(p_data[3]), p_data[4], ft_toInt(p_data[5]));
		players.insert(std::pair<std::string, Player *>(p_data[0], player));
		data.clear();
		p_data.clear();
	}
	file.close();
	return (true);
}

void	Player::save_data(std::map<std::string, Player *> players_list)
{
	std::ofstream file("/Users/mbabela/Desktop/IRC/user.txt");
	std::map <std::string, Player *> players = players_list;
	std::map <std::string, Player *>::iterator	  it ;
	for(it = players.begin(); it != players.end(); it++)
    {
        Player *player;
        player = it->second;
        file << player->getUser()->getNickname() << " " << player->getLevel() << " " << player->getStatus()  << " " << player->getLogtime() << " " << player->getGrad() <<  " " << player->getPoint() << std::endl;
    }
    file.close();
}

bool	Player::check_exist(std::map<std::string, Player *> players, User *user)
{
	std::map<std::string, Player *>::iterator iter;

	std::cout << "checking if the user exist " << std::endl;
	for (iter = players.begin(); iter != players.end(); iter++)
	{
		Player *player;
		player = iter->second;
		if (player->getnickname() == user->getNickname())
		{
			std::cout << "User Already Exist ! " << std::endl;
			return (true);
		}
	}
	std::cout << "User not found " << std::endl;
	return (false);
}

void	Player::add_Points(int point)
{
	this->points += point;
}