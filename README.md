# IRC

    to run :
        first terminal :
            g++ ./srcs/Serverer/Server.cpp -o Server
            ./Server
        second terminal : 
            use *nc* or any equiv command like
                nc -c localhost (127.0.0.1) with the assigned port (9999)
                    : nc -c 127.0.0.1 9999
        same command with the third terminal : 
            you will be able to notice that a multiple users created with a different Id
   
    ******************************************************************************************************************************

   MSG FORMAT   ::

                @id=234AB :dan!d@localhost PRIVMSG #chan :Hey what's up!
                :nickn MODE nickn :+i

                JOIN #hh
                :irc_42!~usern@ll62-82-161-251-62.ll62.iam.net.ma JOIN :#hh
                :punch.wa.us.dal.net 353 irc_42 = #hh :irc_42 @naahio
                :punch.wa.us.dal.net 366 irc_42 #hh :End of /NAMES list.

                KICK irc_42
                :punch.wa.us.dal.net   naahio KICK :Not enough parameters

                PRIVMSH #HH hello
                :naahio!~naahio@ll62-82-161-251-62.ll62.iam.net.ma PRIVMSG #hh :hello
                
                PRIVMSG POPOPOPO :hello
                :naahio!~naahio@ll62-82-161-251-62.ll62.iam.net.ma PRIVMSG POPOPOPO :hello

                KICK #HH irc_42
                :naahio!~naahio@ll62-82-161-251-62.ll62.iam.net.ma KICK #HH irc_42 :naahio

                KICK #HH irc_42
                :punch.wa.us.dal.net 482 POPOPOPO #hh :You're not channel operator

                PART #ATF
                :naahio!~naahio@ll62-82-161-251-62.ll62.iam.net.ma PART #ATF 
        ******************************************************************************************************

        CMD : 
            |-> server : 
                |-> succ => :servname@IP CMD (cmd destination :msg) .
                |-> erro => :servname@IP ERRNO CMD : ERROR
            |-> sender :
                |-> succ => :nick!~user@IP CMD
                |-> erro => server->error
            |-> receiver :
                |-> succ => :send_nick




:user!~user@10.13.6.10 JOIN :#A
:user!~user@10.13.6.10 JOIN :#b


    ******************************************************************************************************************************

    BOT : 
            user_info
            help
            logtime
            
