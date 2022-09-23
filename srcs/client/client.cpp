/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 14:14:58 by ybensell          #+#    #+#             */
/*   Updated: 2022/09/22 16:57:43 by ybensell         ###   ########.fr       */
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
        std::stringstream ss;
        size_t fileSize;    
        int fd;

        ss << vec[4];
        ss >> fileSize;
        vec[3] = vec[3] + ".txt";
        fd = open(vec[3].c_str(),777,O_RDWR | O_CREAT | O_TRUNC);
        std::cout << "getting file ..."  << std::endl;
        char buff[fileSize];
        if (!fd)
        {
            std::cout << "Error while creating a file " << std::endl;
            return ;
        }
        size_t rc = 0;
		while (rc != fileSize)
			rc = recv(sock,buff,fileSize,0);
        for (size_t i = 0 ; i < fileSize ; i++)
        {
            write(fd,buff + i, 1);
        }

    }
}

void sendFile(int sock,std::vector<std::string> &vec)
{
    pid_t pid;

    pid = fork();
    // to change later 
    if (pid == 0)
    {   
        std::cout << "sending file  from Client ..."  << std::endl;
        size_t  fileSize;
        struct stat buf;
        
        fstat(openFiles[0],&buf);
        fileSize = buf.st_size; 
        std::cout << "filesize : " << fileSize << std::endl;
        int reading = 0;
        int offset = 0;
        char buff[fileSize];

        while ((reading = read(openFiles[0],buff,fileSize)) > 0)
        {
            std::cout << "reading : " << reading << std::endl;
        }
        std::cout << "Buffer " << buff << std::endl;
        size_t total = 0;
        while (total != fileSize)
        {
            ssize_t nb = send(sock,buff + total,reading - total, 0);
            if (nb == -1)
                std::cout << "sending error" << std::endl; // to check later 
            total += nb;
        }
    }
}

void  checkFileSending(int sock,std::string &reply)
{
    std::vector<std::string> vec;
    std::stringstream ss;

    size_t fileSize;
    
    split(reply,' ',vec);
    if (!vec[1].compare("NOTICE") && !vec[2].compare("SEND"))
    {
        if (!vec[4].compare("ACCEPT"))
            sendFile(sock,vec);
        else if (!vec[5].compare("ACCEPTED"))
            reciveFile(sock,vec);
    }
}

bool checkPayload(std::string &payload)
{
    int fd;
    std::vector<std::string> vec;
    std::stringstream ss;

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