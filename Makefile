# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hel-makh <hel-makh@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/19 10:46:54 by mbabela           #+#    #+#              #
#    Updated: 2022/08/21 14:45:44 by hel-makh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	serv

HEADERS		=	./srcs/server/server.hpp ./srcs/users/User.hpp

SRCS		=	srcs/main.cpp ./srcs/server/server.cpp ./srcs/users/User.cpp

OBJS		=	$(SRCS:.cpp=.o)

FLAGS		=	-Wall -Wextra -Werror -std=c++98

RM		=	rm -f

%.o:%.cpp	$(HEADERS)
		c++ $(FLAGS) -c $< -o $@

$(NAME):	$(OBJS) $(HEADERS)
		c++ $(FLAGS) $(OBJS) -o $(NAME)

all:		$(NAME)

clean:
		$(RM) $(OBJS)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

.PHONY:		all clean fclean re