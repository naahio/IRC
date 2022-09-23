/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 14:14:58 by ybensell          #+#    #+#             */
/*   Updated: 2022/09/23 14:20:34 by ybensell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/server.hpp"

std::vector<int> openFiles;
int flag = 1;

//:irc!~irc1337 NOTICE SEND tmp ip port size ACCEPTED :  fill will start sending 

void reciveFile(std::vector<std::string> &vec)
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        sleep(1);
        std::stringstream ss;
        size_t fileSize;    
        FILE *fp;

        ss << vec[6];
        ss >> fileSize;
        struct sockaddr_in addr;
        int client,rc;
        uint16_t port;
        ss.clear();
        ss << vec[5];
        ss >> port;
        ss.clear();

        //vec[3] = "recev.txt";
        std::cout << "port : " << port << std::endl;
        std::cout << "ip address :  " << vec[4].c_str() << std::endl;
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
        std::cout << "connected " << std::endl;
        int n;
        size_t nb;
        fp  = fopen(vec[3].c_str(),"w");
        if (!fp)
        {
            std::cout << "cannot open the file" << std::endl;
        }
        char buffer[fileSize];
            nb = 0;
            while (nb < fileSize)
            {
                n = recv(client,buffer,fileSize,0);
                if (n <= 0)
                {
                    std::cout << "errno " << strerror(errno) << std::endl;
                    return ;
                }
                nb += n;
                std::cout << "readead " << n << std::endl;
                std::cout << "buffer " << buffer << std::endl;
            }
            std::cout << "Im here" << std::endl;
            fwrite(buffer,1,fileSize,fp);
            fclose(fp);
            close (client);
            exit (0);
    }
}

void sendFile(std::vector<std::string> &vec)
{
    pid_t pid;

    pid = fork();
    // to change later 
    if (pid == 0)
    {   
        std::cout << "sending file  from Client ..."  << std::endl;
        int server,client;
        struct sockaddr_in serverAdrr,clientAdrr;
        FILE *fp;

        socklen_t addr_size;
        // char hostname[256];
        // struct hostent *host;

        server = socket(AF_INET,SOCK_STREAM,0);
        if (server == -1)
        {
            std::cout << "Error while creating socket" << std::endl;
        }
        memset(&serverAdrr,0,sizeof(serverAdrr));
        serverAdrr.sin_family = AF_INET;
        serverAdrr.sin_port = htons(5555);

        // gethostname(hostname,sizeof(hostname));
        // host = gethostbyname(hostname);
        // to retrieve Ip address We use gethostname and we get the ip from our host
        serverAdrr.sin_addr.s_addr = INADDR_ANY;
        if ((bind(server,(struct sockaddr *)&serverAdrr,sizeof(serverAdrr))) == -1)
        {
            std::cout << "Error while creating sockets" << std::endl;
            return;
        }
        std::cout << "server created " << std::endl;
        if ((listen(server,2)) == -1)
        {
            std::cout << "Error while listening to connectitons"  << std::endl;
            return ;
        }
        addr_size = sizeof(clientAdrr);
        client = accept(server,(struct sockaddr *)&clientAdrr,&addr_size);
        if (client == -1)
        {
            std::cout << "cannot get connection";
        }
        std::cout << "Connection established " << std::endl;
        fp = fopen(vec[3].c_str(),"r");
        if (!fp)
        {
            std::cout << "Cannot open file" << std::endl;
            return ;
        }
        fseek(fp,0,SEEK_END);
        long long fileSize = ftell(fp);
         fseek(fp,0,SEEK_SET);

        int n ;
        char data[fileSize];
        memset(data,0,sizeof(data));
        fread(data,1,fileSize,fp);

        size_t total = 0;
        while (total != fileSize)
        {
            ssize_t nb = send(client,data + total,fileSize - total, 0);
            if (nb == -1)
                std::cout << "sending error" << std::endl; // to check later 
            total += nb;
        }
        fclose(fp);
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
            checkFileSending(reply);
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