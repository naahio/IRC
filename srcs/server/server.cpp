/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 10:53:11 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/23 15:25:20 by hel-makh         ###   ########.fr       */
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

std::map<int, User>	& Server::get_users(void)
{
	return (this->users);
}

void	Server::set_users(std::map <int, User> u_map)
{
	this->users = u_map;
}

int		Server::get_socket_fd(void) const
{
	return (this->socket_fd);
}

struct pollfd *	Server::get_fds(void)
{
	return (this->fds);
}

int		Server::get_nfds(void) const
{
	return (this->nfds);
}

void	Server::set_nfds(int nfds)
{
	this->nfds = nfds;
}

int		Server::get_port(void) const
{
	return (this->port);
}

std::string const &	Server::get_pass(void) const
{
	return (this->password);
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
	struct sockaddr_in	cli;
	int					new_fd = -1;
	
	std::cout << "Waiting for incoming connections . . . " << std::endl;
	do
	{
		new_fd = accept(this->socket_fd, (struct sockaddr *)&cli, (socklen_t *)&cli);
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
		User user = User(inet_ntoa(cli.sin_addr), new_fd);
		this->get_users().insert(std::pair<int, User>(new_fd, user));
		for (std::map<int,User>::iterator itr = this->get_users().begin(); itr != this->get_users().end(); ++itr) {
			std::cout << "map size : " << this->get_users().size() << "\t\tELEMENT : " << std::endl;
			std::cout << itr->first << '\t' << itr->second.get_ip() << "\t" << itr->second.get_fd() << '\n';
		}
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
		std::cout << rc << " bytes received." << std::endl;
		std::cout << "Message received: " << this->buffer;
		send(fd, "received succ >.<\n", sizeof("received succ >.<\n"), 0);
	} while (true);
	return (true);
}
