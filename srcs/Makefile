# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/19 10:46:54 by mbabela           #+#    #+#              #
#    Updated: 2022/08/21 14:24:29 by mbabela          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	serv

HEADERS		=	./server/server.hpp ./users/User.hpp

SRCS		=	main.cpp ./server/server.cpp ./users/User.cpp

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