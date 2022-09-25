import socket
import sys

from numpy import size

class lily:
    
    socket = socket.socket();
    
    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    def send_msg(self, reciever, msg):
        self.socket.send(bytes("PRIVMSG " + reciever + " " + msg + "\n"))
    
    def serv_connect(self, server, port, username, password, nickname, channel):
        print("Connecting to " + server)
        self.socket.connect((server, port))
        
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
        with open('/Users/mbabela/Desktop/IRC/user.txt') as f:
            lines = f.readlines()
        return lines
        

irc_serv = "10.12.8.5"
irc_port = 9999
channel  = "#General"
bot_user = "bot"
bot_nick = "/lily"
password = "1337"

irc = lily()
irc.serv_connect(irc_serv, irc_port, bot_user, password, bot_nick, channel)

while True:
    message = irc.serv_response()
    print("recieved : \n" + message)
    
    if "/lily" in message:
        irc.send_msg(channel, ":this is lily, how can i help you ?")
        users = irc.read_file()
        i = 0
        for i in users:
            print("users : " + i)
        break
    