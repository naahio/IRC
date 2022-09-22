/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 14:14:58 by ybensell          #+#    #+#             */
/*   Updated: 2022/09/22 13:17:07 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"

std::vector<int> openFiles;
int flag = 1;

void reciveFile(int sock,std::vector<std::string> &vec)
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        
    }
}

void sendFile(int sock,std::vector<std::string> &vec)
{

}

void  checkFileSending(int sock,std::string &reply)
{
    std::vector<std::string> vec;

    split(reply,' ',vec);
    if (!vec[1].compare("NOTICE") && !vec[2].compare("SEND"))
    {
        if (!vec[4].compare("ACCEPT"))
            reciveFile(sock,vec);
        else if (!vec[4].compare("ACCEPTED"))
            sendFile(sock,vec);
    }
}

bool checkPayload(std::string &payload)
{
    int fd;
    std::vector<std::string> vec;
    std::stringstream ss;

    split(payload,' ',vec);
    if (!vec[0].compare("SEND"))
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
        payload += " " + fsize;
        openFiles.push_back(fd);
    }
    return true;

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
    pid_t pid;
    std::string reply;
    pid = fork();
    if (pid == 0)
    {
        while (1)
        {
            memset(buff,0,BUFF_SIZE);

            rc = recv(sock,buff,sizeof(buff),0);
            if (rc == 0)
            {
                std::cout << "Server closed " << std::endl;
                close(sock);
                flag = 0;
                return 1;
            }
            std::cout << buff << std::endl ;
            reply = buff;
            checkFileSending(sock,reply);


        }
    }
    while (flag)
    {
        std::getline(std::cin,payload);
        if (!checkPayload(payload))
        {
            payload.clear();
            continue;
        }
        payload += "\r\n";
        std::cout << "payload :" <<  payload << std::endl;
        sendReply(sock,payload);
        payload.clear();
    }
    close (sock);
}