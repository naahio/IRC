/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 10:13:49 by mbabela           #+#    #+#             */
/*   Updated: 2022/09/10 15:49:15 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../server/server.hpp"

void    helps(int fd)
{
    send(fd, ":IRC_1337 NOTICE HELP :use te following commands to register or log in : \n", sizeof(":IRC_1337 NOTICE HELP :use te following commands to register or log in : \n"), 0);
    send(fd, "|-> <PASS> 'password' \n", sizeof("|-> <PASS> 'password' \n"), 0);
    send(fd, "|-> <USER> 'username' 'hostname' 'servername' 'realname' \n", sizeof("|-> <USER> 'username' 'hostname' 'servername' 'realname' \n"), 0);
    send(fd, "|-> <NICK> 'nickname' \n", sizeof("|-> <NICK> 'nickname' \n"), 0);
    send(fd, "Channel commands : \n", sizeof("Channel commands : \n"), 0);
    send(fd, "|-> <JOIN>  '<#>channel name'. . . 'key'. . . \n", sizeof("|-> <JOIN>  '<#>channel name'. . . 'key'. . . \n"), 0);
    send(fd, "|-> <KICK>  '<#>channel name'. . . 'user'. . . \n", sizeof("|-> <KICK>  '<#>channel name'. . . 'user'. . . \n"), 0);
    send(fd, "|-> <PART>  '<#>channel name' \n", sizeof("|-> <PART>  '<#>channel name' \n"), 0);
    send(fd, "|-> <MODE>  '<#>channel name' <mode> \n", sizeof("|-> <MODE>  '<#>channel name' <mode> \n"), 0);
    send(fd, "|-> <USERMODES>  'mode' \n", sizeof("|-> <USERMODES>  'mode' \n"), 0);
    send(fd, "|-> <LIST>  '<#>channel name' 'server' \n", sizeof("|-> <LIST>  '<#>channel name' 'server' \n"), 0);
    send(fd, "|-> <INVITE>  'nickname' '<#>channel name' \n", sizeof("|-> <INVITE>  'nickname' '<#>channel name' \n"), 0);
    send(fd, "Server commands : \n", sizeof("Server commands : \n"), 0);
    send(fd, "|=> <VERSION> \n", sizeof("|=> <VERSION> \n"), 0);
    send(fd, "|=> <TIME>  \n", sizeof("|=> <TIME>  \n"), 0);
    send(fd, "|=> <ADMINE> \n", sizeof("|=> <ADMINE> \n"), 0);
    send(fd, "|=> <INFO>  \n", sizeof("|=> <INFO>  \n"), 0);
    send(fd, "|=> <KILL> 'nickname' 'reason' \n", sizeof("|=> <KILL> 'nickname' 'reason' \n"), 0);
    send(fd, "Cient to client / channel commandes : \n", sizeof("Cient to client / channel commandes : \n"), 0);
    send(fd, "|-> <PRIVMSG> 'receiver' 'message' \n", sizeof("|-> <PRIVMSG> 'receiver' 'message' \n"), 0);


    // std::cout << "use te following commands to register or log in : " << std::endl;
    // std::cout << "|-> <PASS> 'password' " << std::endl;
    // std::cout << "|-> <USER> 'username' 'hostname' 'servername' 'realname'" << std::endl;
    // std::cout << "|-> <NICK> 'nickname' " << std::endl;
    // std::cout << "Channel commands : " << std::endl;
    // std::cout << "|-> <JOIN>  '<#>channel name'. . . 'key'. . ." <<std::endl;
    // std::cout << "|-> <KICK>  '<#>channel name'. . . 'user'. . ." <<std::endl;
    // std::cout << "|-> <PART>  '<#>channel name'" <<std::endl;
    // std::cout << "|-> <MODE>  '<#>channel name' <mode>" <<std::endl;
    // std::cout << "|-> <USERMODES>  'mode'" <<std::endl;
    // std::cout << "|-> <USERMODES>  'mode'" <<std::endl;
    // std::cout << "|-> <LIST>  '<#>channel name' 'server'" <<std::endl;
    // std::cout << "|-> <INVITE>  'nickname' '<#>channel name'" <<std::endl;
    // std::cout << "Server commands : " << std::endl;
    // std::cout << "|=> <VERSION> " << std::endl;
    // std::cout << "|=> <TIME> " << std::endl;
    // std::cout << "|=> <ADMINE> " << std::endl;
    // std::cout << "|=> <INFO> " << std::endl;
    // std::cout << "|=> <KILL> 'nickname' 'reason' " << std::endl;
    // std::cout << "Cient to client / channel commandes : " << std::endl;
    // std::cout << "|-> <PRIVMSG> 'receiver' 'message'" <<std::endl;

}

// void    kick(int fd)
// {

// }

// void    part(int fd)
// {
    
// }