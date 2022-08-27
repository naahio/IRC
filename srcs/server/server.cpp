/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 10:53:11 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/27 17:14:14 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "server.hpp"

Server::Server(int _port, std::string _password)
{
	std::cout << "Creating Server . . . " << std::endl;
	std::cout << "updating parameter . . . " << std::endl;
	this->port = _port;
	this->password = _password;
	this->nfds = 0;
	std::cout << "Server created, password : " << this->password << std::endl;
}

Server::~Server(void)
{
	std::cout << "Server deleted." << std::endl;
}

std::map<std::string, User *>	& Server::getUsers(void)
{
	return (this->users);
}

std::map<int, User *>	& Server::getGuests(void)
{
	return (this->guests);
}

int		Server::getSocketFd(void) const
{
	return (this->socket_fd);
}

struct pollfd *	Server::getFds(void)
{
	return (this->fds);
}

int		Server::getNfds(void) const
{
	return (this->nfds);
}

void	Server::setNfds(int nfds)
{
	this->nfds = nfds;
}

int		Server::getPort(void) const
{
	return (this->port);
}

std::string const &	Server::getPass(void) const
{
	return (this->password);
}

void	Server::addGuest(int fd)
{
	User *	guest;

	guest = new User(fd);
	this->guests.insert(std::pair<int, User *>(fd, guest));
}

void	Server::deleteGuest(int fd)
{
	std::map<int, User *>::iterator it;

	it = this->guests.find(fd);
	if (it != this->guests.end()) {
		delete it->second;
		this->guests.erase(it);
	}
}

void	Server::addUser(User * user)
{
	this->users.insert(std::pair<std::string, User *>(user->getUsername(), user));
}

void	Server::deleteUser(std::string username)
{
	std::map<std::string, User *>::iterator it;

	it = this->users.find(username);
	if (it != this->users.end()) {
		delete it->second;
		this->users.erase(it);
	}
}

int		Server::Create_socket(void)
{
	std::cout << "Creating socket . . . ";
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->socket_fd < 0)
	{
		std::cout << "[FAILED: " << errno << "]" << std::endl;
		return (0);
	}
	this->fds[0].fd = this->socket_fd;
	this->nfds++;
	std::cout << "[DONE]" << std::endl;
	return (1);
}

int		Server::reusable_socket(void)
{
	int	rc;
	
	std::cout << "Setting up socket options . . . ";
	rc = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&this->on, sizeof(this->on));
	if (rc < 0)
	{
		std::cout << "[FAILED: " << errno << "]" << std::endl;
		return (0); 
	}
	std::cout << "[DONE]" << std::endl;
	return (1);
}

int		Server::nonblocking_socket(void)
{
	int	rc;
	
	std::cout << "Making socket non blocking . . . ";
	rc = fcntl(this->socket_fd, F_SETFL, O_NONBLOCK);
	if (rc < 0)
	{
		std::cout << "[FAILED: " << errno << "]" << std::endl;
		return (0);
	}
	std::cout << "[DONE]" << std::endl;
	return (1);
}

int		Server::bind_socket(void)
{
	struct sockaddr_in	addr;
	int					rc;
	
	std::cout << "Binding Socket . . . ";
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(this->port);
	rc = bind(this->socket_fd, (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0)
	{
		std::cout << "[FAILED: " << errno << "]" << std::endl;
		return (0);
	}
	std::cout << "[DONE]" << std::endl;
	return (1);
}

int		Server::listen_from_socket(void)
{
	int	rc;
	
	std::cout << "Listening . . ." << std::endl;
	rc = listen(this->socket_fd, MAX_CONN);
	if (rc < 0)
	{
		std::cout << "[FAILED: " << errno << "]" << std::endl;
		return (0);
	}
	return (1);
}

void	Server::poll_trait(void)
{
	std::cout << "Setting up poll structure . . . ";
	memset(this->fds, 0, sizeof(this->fds));
	this->fds[0].fd = this->socket_fd;
	this->fds[0].events = POLLIN;
	std::cout << "[DONE]" << std::endl;
}

bool	Server::accept_connections(void)
{
	int	new_fd = -1;
	
	std::cout << "Waiting for incoming connections . . . " << std::endl;
	do
	{
		new_fd = accept(this->socket_fd, NULL, NULL);
		if (new_fd == -1)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cout << "FAILED at accepting connection ! errno : " << errno << std::endl;
				return (false);
			}
			break;
		}
		std::cout << "NEW Connection detected " << new_fd << std::endl;
		this->addGuest(new_fd);
		this->fds[this->nfds].fd = new_fd;
		this->fds[this->nfds].events = POLLIN;
		this->nfds++;
	} while (new_fd != -1);
	return (true);
}

bool	Server::recv_send_msg(int fd)
{
	int	rc;

	std::cout <<  "Receiving message . . ." << std::endl;
	do
	{
		rc = recv(fd, this->buffer, sizeof(this->buffer), 0);
		if (rc == -1)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cout << "FAILED at receiving a msg ! errno : " << errno << std::endl;
				return (false);
			}
			break ;
		}
		if (rc == 0)
		{
			std::cout << "Connection closed . . . " << std::endl;
			return (false);
		}
		this->buffer[rc] = '\0';
		Msg msg = Msg(buffer, fd);
		
		send(fd, "received succ >.<\n", sizeof("received succ >.<\n"), 0);
	} while (true);
	return (true);
}
