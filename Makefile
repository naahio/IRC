# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/19 10:46:54 by mbabela           #+#    #+#              #
#    Updated: 2022/09/28 11:35:05 by hel-makh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ircserv

CLIENT_NAME	=	client

HEADERS		=	./srcs/server/server.hpp\
				./srcs/users/User.hpp\
				./srcs/pars/msg.hpp\
				./srcs/channels/Channel.hpp\
				./srcs/tools/replies.hpp\
				./srcs/bot/player/Player.hpp\
				./srcs/tools/tool.hpp\
				./srcs/tools/replies.hpp

SRCS		=	./srcs/main.cpp\
				./srcs/server/server.cpp\
				./srcs/users/User.cpp\
				./srcs/pars/msg.cpp\
				./srcs/channels/Channel.cpp\
				./srcs/bot/player/Player.cpp\
				./srcs/tools/replies.cpp\
				./srcs/tools/Commands.cpp\
				./srcs/tools/tool.cpp

CLIENT_SRCS	=	./srcs/client/ClientV2.cpp

OBJS		=	$(SRCS:.cpp=.o)

FLAGS		=	-Wall -Wextra -Werror -std=c++98

RM			=	rm -f

%.o:%.cpp	$(HEADERS)
			c++ $(FLAGS) -c $< -o $@

$(NAME):	$(OBJS) $(HEADERS)
			c++ $(FLAGS) $(OBJS) -o $(NAME)

client:
			c++ $(FLAGS) $(CLIENT_SRCS) -o $(CLIENT_NAME)

bot:
			python srcs/bot/lily.py

all:		$(NAME) client

clean:
			@$(RM) $(OBJS)

fclean:		clean
			@$(RM) $(NAME) $(CLIENT_NAME)
re:			fclean all

.PHONY:		all clean fclean re