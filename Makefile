# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybensell <ybensell@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/19 10:46:54 by mbabela           #+#    #+#              #
#    Updated: 2022/09/25 13:36:10 by ybensell         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ircserv

HEADERS		=	./srcs/server/server.hpp\
				./srcs/users/User.hpp\
				./srcs/pars/msg.hpp\
				./srcs/channels/Channel.hpp\
				./srcs/tools/tool.hpp\
				./srcs/tools/replies.hpp

SRCS		=	srcs/main.cpp\
				./srcs/server/server.cpp\
				./srcs/users/User.cpp\
				./srcs/pars/msg.cpp\
				./srcs/channels/Channel.cpp\
				./srcs/tools/tool.cpp\
				./srcs/tools/replies.cpp\
				./srcs/tools/Commands.cpp

OBJS		=	$(SRCS:.cpp=.o)

FLAGS		=	-Wall -Wextra -Werror -std=c++98

RM			=	rm -f

CLIENT		= ./srcs/client/ClientV2.cpp

%.o:%.cpp	$(HEADERS)
			c++ $(FLAGS) -c $< -o $@

$(NAME):	$(OBJS) $(HEADERS)
			c++ $(FLAGS) $(OBJS) -o $(NAME)

all:		$(NAME)

clean:
			@$(RM) $(OBJS)

fclean:		clean
			@$(RM) $(NAME)

client:
			c++ $(FLAGS) $(CLIENT) -o client
re:			fclean all

.PHONY:		all clean fclean re