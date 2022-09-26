import socket

class player:

	def __init__(self):
		self.fd			= 0
		self.nickname	= ""
		self.post		= ""
		self.level		= ""
		self.status 	= ""
		self.logtime	= ""
		self.rank 		= ""
		self.points 	= 0

	def creat_player(self, fd, nickname, post, level, status, logtime, rank, points):
		self.fd			= fd
		self.nickname	= nickname
		self.post		= post;
		self.level		= level
		self.status 	= status
		self.logtime	= logtime
		self.rank 		= rank
		self.points 	= points

class sender:
    
    def __init__(self):
        self.nickname = ""

  
class lily:
    
    socket = socket.socket()
    
    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    def send_msg(self, reciever, msg):
        self.socket.send(bytes("PRIVMSG " + reciever + " :" + msg + "\n"))
    
    def serv_connect(self, server, port, username, password, nickname, channel):
        print("Connecting to " + server)
        if self.socket.connect((server, port)):
            self.socket.send(bytes("PASS " + password + "\n"))
        self.socket.send(bytes("USER " + username + " *" + " *" + " *" + "\n"))
        self.socket.send(bytes("NICK " + nickname + "\n"))
        
        self.socket.send(bytes("JOIN " + channel + "\n"))
        
    def serv_response(self):
        resp = self.socket.recv(1024).decode("UTF-8")
        if resp.find('PING') != -1:
            self.socket.send(bytes('PONG ' + resp.split().decode("UTf-8") [1] + '\r\n'))
        return resp

    def read_file(self):
        user_list = {}
        with open("/Users/mbabela/Desktop/IRC/user.txt") as csv_file:
            results = []
            for line in csv_file:
                words = line.split()
                results.append((words[0:]))
            for fd, post, ip, nickname, username in results:
                player = player()
                player.creat_player(fd, ip, post, nickname, username)
                user_list[fd] = player
        return user_list
    
    def fin_user(self, looking_for, user_list, sender):
        i = 1
        for u in user_list:
            player = user_list[u]
            if player.nickname == looking_for:
                i = 0
                self.send_msg(sender, player.post) 
                break
        if i == 1:
            self.send_msg(sender, "USER NOT FOUND !") 
    
    def pars_message(message):
        splt = message.split()   
                

IRC_SERV 	=	"10.12.8.5"
IRC_PORT 	=	9999
CHANNEL  	=	"#General"
BOT_USER 	=	"bot"
BOT_NICK	=	"/lily"
PASSWORD 	=	"1337"

irc = lily()
irc.serv_connect(IRC_SERV, IRC_PORT, BOT_USER, PASSWORD, BOT_NICK, CHANNEL)
user_list = {}
player = player()

while True:
    message = irc.serv_response()
    if "WHERE" in message:
        cmd = message.split("!")
        cmd = cmd[0].split(":")
        irc.send_msg(cmd[1],"Looking for the USER . . .")
        w = message.split()
        irc.fin_user(w[2], user_list, cmd[1])
    elif "L_DAPET" in message:
        user_list = irc.read_file()
        print("-*----------*-")
        for u in user_list:
            print(user_list[u].nickname)
    elif "PRIVMSG" in message:
        cmd = message.split("!")
        cmd = cmd[0].split(":")
        irc.send_msg(cmd[1], "cmd available [find nickname] ! waiting for the commands from the dev . . .")
    if not message:
    	break
