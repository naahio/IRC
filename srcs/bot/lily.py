import socket
# from google_trans_new import google_translator
# from googletrans import Translator
# translator = Translator()

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
		self.post		= post
		self.level		= level
		self.status 	= status
		self.logtime	= logtime
		self.rank 		= rank
		self.points 	= points
 
class lily:
    
    socket = socket.socket()
    
    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    def send_msg(self, reciever, msg):
        self.socket.send(bytes("PRIVMSG " + reciever + " :" + msg + "\n"))
    
    def serv_connect(self, server, port, username, password, nickname, general, random):
        print("Connecting to " + server)
        self.socket.connect((server, port))
        
        self.socket.send(bytes("PASS " + password + "\n"))
        self.socket.send(bytes("USER " + username + " *" + " *" + " *" + "\n"))
        self.socket.send(bytes("NICK " + nickname + "\n"))
        
        self.socket.send(bytes("JOIN " + general + "\n"))
        self.socket.send(bytes("JOIN " + random + "\n"))
        self.socket.send(bytes("TOPIC " + general + " :Talk about all the different things .\n"))
        self.socket.send(bytes("TOPIC " + random + " :you can troll as much as you want, disrespect = banne \n"))
        
    def serv_response(self):
        resp = self.socket.recv(1024).decode("UTF-8")
        if resp.find('PING') != -1:
            self.socket.send(bytes('PONG ' + resp.split().decode("UTf-8") [1] + '\r\n'))
        return resp

    def read_file(self):
        user_list = {}
        with open("user.txt") as csv_file:
            results = []
            for line in csv_file:
                words = line.split()
                results.append((words[0:]))
            for fd, nickname, post, level, status, logtime, rank, points in results:
                playr = player()
                playr.creat_player(fd, nickname, post, level, status, logtime, rank, points)
                user_list[nickname] = playr
            print(user_list)
        return user_list
    
    def get_player(self, sender):
        i = 1
        for u in user_list:
            player = user_list[u]
            if player.nickname == sender:
                i = 0
                return player
        if i == 1:
            self.send_msg(sender, "USER NOT FOUND !")
            return None

    def pars_message(self, message):
        splt = message.split('!')
        splt = splt[0].split(':')
        return splt[1]

    def fin_user(self, looking_for, user_list, sender):
        i = 1
        for u in user_list:
            player = user_list[u]
            if player.nickname == looking_for:
                i = 0
                pst = player.post.split('.')
                self.send_msg(sender, pst[0]) 
                break
        if i == 1:
            self.send_msg(sender, "(X) USER NOT FOUND ! (~_~)")

    def palyer_profile(self, sender):
        player = self.get_player(sender)
        if player:
            print("Nickname	: " + player.nickname)
            print("Level    : " + player.level)
            print("Status   : " + player.status)
            print("LogTime  : " + player.logtime)
            print("Rank     : " + player.rank)
            print("Points   : " + player.points)
            self.send_msg(sender, "*********[Profile]*******\n")
            self.send_msg(sender, ("***[	Nickname =	" + player.nickname) + "	]***")
            self.send_msg(sender, ("***[	Level    =	" + player.level) + "	]*")
            self.send_msg(sender, ("***[	Status   =	" + player.status) + "	]**")
            self.send_msg(sender, ("***[	LogTime  =	" + player.logtime) + "	]****")
            self.send_msg(sender, ("***[	Rank     =	" + player.rank) + "	]**")
            self.send_msg(sender, ("***[	Points   =	" + player.points) + "	]******")
            self.send_msg(sender, "**************************\n")
        else :
            self.send_msg(sender, "NO DATA ABOUT THE USER  !")

HOSTNAME    =   socket.gethostname()   
IRC_SERV    =   socket.gethostbyname(HOSTNAME)
IRC_PORT 	=	6667
GENERAL  	=	"#General"
RANDOM  	=	"#Random"
BOT_USER 	=	"bot"
BOT_NICK	=	"/lily"
PASSWORD 	=	"123"

irc = lily()
irc.serv_connect(IRC_SERV, IRC_PORT, BOT_USER, PASSWORD, BOT_NICK, GENERAL, RANDOM)
user_list = {}
playr = player()

while True:
    message = irc.serv_response()
    if message:
        print("message : " +message)
        sender = irc.pars_message(message)

    if "whereis" in message:
        irc.send_msg(sender,"Looking for the USER . . .")
        w = message.split()
        if len(w) <= 4:
            irc.send_msg(sender, "(X) USER NOT FOUND ! (~_~)")
        else:
            irc.fin_user(w[4], user_list, sender)
    elif "profile" in message:
        irc.palyer_profile(sender)
    elif "L_DAPET" in message:
        user_list = irc.read_file()
        print("-*----------*-")
        for u in user_list:
            print(user_list[u].nickname)
    elif "help" in message:
        cmd = message.split("!")
        cmd = cmd[0].split(":")
        irc.send_msg(cmd[1], "this is lily")
        irc.send_msg(cmd[1], "|->please use > ")
        irc.send_msg(cmd[1], "|----> profile => to see your profile")
        irc.send_msg(cmd[1], "|----> whereis nickname => to find a player")
    elif "thank" in message:
        irc.send_msg(sender, "you are Welcome (^_^)")
    # elif "translate" in message:
    #     text = message.split()
    #     result = translator.translate(text[5], dest=text[4])
    #     irc.send_msg(sender, result)
    if not message:
    	break


#:lime!~limeChat@10.12.8.5   0
# PRIVMSG					 1
# /lily						 2
# :translate(pattern)		 3
# jp  						 4
# <hello>					 5