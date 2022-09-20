<?php


set_time_limit(0);

$channels   = array('General', 'Random');

$nickname   = '/lily';
$username   = 'bot';
$password   = '1337';

$master     = 'admin';

$socket     = fsockopen("10.12.8.5", 9999);

fputs($socket, "PASS " . $password . "\n");
fputs($socket, "NICK " . $nickname . "\n");
fputs($socket, "USER " . $username . " *" . " *" . " *" . "\n");


foreach($channels as $channel)  {
    fputs($socket, "JOIN #". $channel . "\n");
}

while (1)
{
    while ($data = fgets($socket, 1024)) {
        echo $data; //data = :naahio!~naahio@127.0.0.1 PRIVMSG ∂bot/lily :!help
        flush();
        $ex = explode(' ', $data);
        $part = explode("!", $ex[0]);
        $sender = substr($part['0'], 1, strlen($part[0]));
        if ($ex[0] == 'PING') {
            fputs($socket, "PONG" . $ex[1] . "\n");
        }
        else if (ISSET($ex[3])) {
            $command = str_replace(array(
                chr(10),
                chr(13)
            ), '', $ex[3]);
            echo "command : [ " .$command . "]\n";
            if ($command == ":!alive?") {
                fputs($socket, "PRIVMSG " . $sender . " :ofc what do you think ? \n");
            }    
            if ($command == ":!help") {
                fputs($socket, "PRIVMSG " . $sender . " :lily bot v0.0 commands \n");
                fputs($socket, "PRIVMSG " . $sender . " :!alive?, !time, !help, !maden, !meat, !meme, !chucknorris \n");
            }    
            if ($command == ":!time") {
                fputs($socket, "PRIVMSG " . $sender . ": " . date(DATE_RFC2822) ." \n");
            }    
            if ($command == ":!maden") {
                $part = explode("!", $ex[0]);
                $user = substr($part['0'], 1, strlen( $part['0']));

                if ($user == $master)
                    fputs($socket, "PRIVMSG " . $user . " :Yes master ! \n");
                else
                    fputs($socket, "PRIVMSG " . $user . " :get lost " . $user . " you lowly being! \n");
            }
            if ($command == ":!chucknorris") {
                $joke = json_decode(file_get_contents('http://api.icndb.com/jokes/random', true));
                fputs($socket, "PRIVMSG " . $sender . " :" . $joke->value->joke . " \n");
            }    
            if ($command == ":!meme") {
                $meme = file_get_contents('/Users/mbabela/Desktop/IRC/srcs/bot/test.txt');
                fputs($socket, "PRIVMSG " . $sender . " :" . $meme . " \n");
            }    
            if ($command == ":!meat") {
                $meat = file_get_contents('/Users/mbabela/Desktop/IRC/srcs/bot/test.txt');
                fputs($socket, "PRIVMSG " . $sender . " :" . $meat . " \n");
            }    
        }
    }
}