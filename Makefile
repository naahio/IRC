# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/19 10:46:54 by mbabela           #+#    #+#              #
#    Updated: 2022/08/23 09:32:36 by mbabela          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ircserv

HEADERS		=	./srcs/server/server.hpp ./srcs/users/User.hpp ./srcs/tools/tool.hpp

SRCS		=	srcs/main.cpp ./srcs/server/server.cpp ./srcs/users/User.cpp ./srcs/tools/check_args.cpp

OBJS		=	$(SRCS:.cpp=.o)

FLAGS		=	-Wall -Wextra -Werror -std=c++98

RM			=	rm -f

%.o:%.cpp	$(HEADERS)
			c++ $(FLAGS) -c $< -o $@

$(NAME):	$(OBJS) $(HEADERS)
			c++ $(FLAGS) $(OBJS) -o $(NAME)

all:		$(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re