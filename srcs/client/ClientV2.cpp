/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientV2.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 14:14:58 by ybensell          #+#    #+#             */
/*   Updated: 2022/09/25 09:56:11 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <vector>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/poll.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <netinet/in.h>
# include <errno.h>
# include <iostream>
# include <unistd.h>
# include <cstring>
# include <iterator>
# include <map>
# include <fcntl.h>
# include <arpa/inet.h>
# include <fstream>
# include <sstream>
# include <ctime>
# include <sys/types.h>
# include <sys/stat.h>
# include <netdb.h>


int flag = 1;

// to do : handle errors 
//:irc!~irc1337 NOTICE SEND tmp ip port size ACCEPTED :  fill will start sending 

void	split(std::string const &s1,char delim,
				std::vector<std::string> &out)
{
	std::stringstream X(s1);
	std::string T;

	while (std::getline(X,T,delim))
	{
		if (!T.empty())
			out.push_back(T);
	}
}

void	sendReply(int fd,const std::string &reply)
{
	size_t total = 0;
	while (total != reply.length())
	{
		ssize_t nb = send(fd,reply.c_str() + total,reply.length() - total, 0);
		if (nb == -1)
			std::cout << "sending error" << std::endl; // to check later 
		total += nb;
	}
}

void reciveFile(std::vector<std::string> &vec)
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        sleep(1);
        std::stringstream ss;
        size_t fileSize;    
        int fd;

        ss << vec[6];
        ss >> fileSize;
        struct sockaddr_in addr;
        int client,rc;
        uint16_t port;
        ss.clear();
        ss << vec[5];
        ss >> port;
        ss.clear();
        std::cout << "Getting the file ...." << std::endl;
        std::cout << "ip address : " << vec[4].c_str() << std::endl;
        std::cout << "port       : " << port << std::endl;

        if ((client = socket(AF_INET,SOCK_STREAM,0)) < 0)
        {
            std::cout << "Error while creating sockets " << std::endl;
            return ;
        }

        memset(&addr,0,sizeof(addr));
        addr.sin_family = AF_INET;
        if (inet_pton(AF_INET, vec[4].c_str(),&(addr.sin_addr)) < 0)
        {
            std::cout << "Server not availabe" << std::endl;
            return ;
        }
        addr.sin_port = htons(port);
        
        if ((rc = connect(client,(struct sockaddr *)&addr,sizeof(addr))) < 0)
        {
            std::cout << "can not connect to server" << std::endl;
            return ;
        }
        std::cout << "connection established .... " << std::endl;
        int n;
        size_t nb;
        fd  = open(vec[3].c_str(),O_RDWR | O_CREAT | O_APPEND,0644);
        if (!fd)
        {
            std::cout << "cannot open the file" << std::endl;
            close(client);
            exit(0);
        }
        
        char buffer[1];
        nb = 0;
      
        while (nb < fileSize)
        {  
            memset(buffer,0,sizeof(buffer));
            n = recv(client,buffer,1,0);
            if (n <= 0)
            {
                std::cout << "errno " << strerror(errno) << std::endl;
                return ;
            }
            nb += n;
            write(fd,buffer,1);
        }
        std::cout << "total byte recieved :  " << nb << std::endl;
        std::cout << "The file has been recieved succefully :D" << std::endl;
        close(fd);
        close (client);
        exit (0);
    }
}

void sendFile(std::vector<std::string> &vec)
{
    pid_t pid;

    pid = fork();

    if (pid == 0)
    {   
        std::cout << "sending file  from Client ...."  << std::endl;
        int server,client;
        struct sockaddr_in serverAdrr,clientAdrr;
        int fd;
        socklen_t addr_size;


        server = socket(AF_INET,SOCK_STREAM,0);
        if (server == -1)
        {
            std::cout << "Error while creating socket" << std::endl;
            exit(0);
        }
        memset(&serverAdrr,0,sizeof(serverAdrr));
        serverAdrr.sin_family = AF_INET;
        serverAdrr.sin_port = htons(5555);
        serverAdrr.sin_addr.s_addr = INADDR_ANY;
        if ((bind(server,(struct sockaddr *)&serverAdrr,sizeof(serverAdrr))) == -1)
        {
            std::cout << "Error while creating sockets" << std::endl;
            close(server);
            exit(0);
        }
        std::cout << "Server Created ...."  << std::endl;
        if ((listen(server,2)) == -1)
        {
            std::cout << "Error while listening to connectitons"  << std::endl;
            close(server);
            exit(0);
        }
        addr_size = sizeof(clientAdrr);
        client = accept(server,(struct sockaddr *)&clientAdrr,&addr_size);
        if (client == -1)
        {
            std::cout << "cannot get connection";
            close(server);
            exit(0);
        }
        std::cout << "Connection established ...." << std::endl;
        fd = open(vec[3].c_str(),O_RDONLY);
        if (!fd)
        {
            std::cout << "Cannot open file " << std::endl;
            close(server);
            close(client);
            exit(0);
        }
    
        __int64_t	fileSize;
        struct stat buf;
        fstat(fd,&buf);
    
        fileSize = buf.st_size;;

        int n ;
        char data[1];
       
        n = 1;
        size_t total = 0;
        while ((read(fd,data,1)) != 0)
        {
            ssize_t nb = send(client,data ,1, 0);
            if (nb == -1)
                std::cout << "sending error" << std::endl; // to check later 
            total += nb; 
            memset(data,0,sizeof(data));
        }
        std::cout << "Total byte sent : " << total << std::endl;
        std::cout << "The file has been sent "  << std::endl;
        close(fd);
        close(client);
        close(server);
        exit (0);
    }
}

void  checkFileSending(std::string &reply)
{
    std::vector<std::string> vec;
    std::stringstream ss;
    size_t fileSize;

    split(reply,' ',vec);
    if (!vec[1].compare("NOTICE") && !vec[2].compare("SEND"))
    {
        if (!vec[4].compare("ACCEPT"))
            sendFile(vec);
        else if (!vec[7].compare("ACCEPTED"))
            reciveFile(vec);
    }
}

bool checkPayload(std::string &payload)
{
    int fd;
    std::vector<std::string> vec;
    std::stringstream ss;

    if (payload.empty())
        return true;
    split(payload,' ',vec);
    if ( vec[0].length() != 0 && (!vec[0].compare("SEND")))
    {
        if (vec.size() < 3)
        {
            std::cout << "Error need the file before sending it " << std::endl;
            return false;
        }
        fd = open(vec[2].c_str(),O_RDONLY);
        if (fd == -1)
        {
            std::cout << "Error cannot open the file " << std::endl;
            return false;
        }

        __int64_t	fileSize;
        struct stat buf;
        fstat(fd,&buf);
        ss << buf.st_size;
        std::string fsize;
        ss >> fsize;

        payload += " 5555 "; 
        payload +=  fsize;
        close(fd);
    }
    return true;

}

void    *reciving(void *arg)
{
    int rc;
    int *sock = (int *)arg; 
    char buff[1024];
    std::string reply;
    while (flag)
    {
        memset(buff,0,1024);
        rc = recv(*sock,buff,sizeof(buff),0);
        if (rc == 0)
        {
            std::cout << "Server closed " << std::endl;
            close(*sock);
            flag = 0;
            exit(0) ;
        }
        std::cout << buff ;
        reply = buff;
        checkFileSending(reply);
    }
    return NULL;
}

int main(int argc,char **argv)
{
    if (argc != 3 )
        return 1;
    std::stringstream ss;
    struct sockaddr_in addr;
    int sock,client,rc;
    uint16_t port;
    std::string payload;
    char buff[1024];
    pid_t pid;
    std::string reply;

    ss << argv[2];
    ss >> port;

    if ((sock = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        std::cout << "Error while creating sockets " << std::endl;
        return 1;
    }
    
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, argv[1],&(addr.sin_addr)) < 0)
    {
        std::cout << "Server not availabe" << std::endl;
        return 1;
    }
    addr.sin_port = htons(port);
    
    if ((client = connect(sock,(struct sockaddr *)&addr,sizeof(addr))) < 0)
    {
        std::cout << "can not connect to server" << std::endl;
        return 1;
    }
    pthread_t id;
    pthread_create(&id,NULL,reciving,&sock);
    while (flag)
    {
        std::getline(std::cin,payload);
        if (!checkPayload(payload))
        {
            payload.clear();
            continue;
        }
        payload += "\r\n";
        sendReply(sock,payload);
        payload.clear();
    }
    close (sock);
}