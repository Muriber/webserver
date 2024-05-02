# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaromero <jaromero@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/02 23:06:05 by jaromero          #+#    #+#              #
#    Updated: 2023/11/25 23:59:11 by jaromero         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= webserv

SOURCES	=	srcs/main.cpp srcs/Server.cpp srcs/SrvConfigFile.cpp \
			srcs/Response.cpp srcs/CFGcluster.cpp srcs/CFGlocation.cpp \
			srcs/ParseData.cpp srcs/CGIexec.cpp

OBJECTS	=	$(SOURCES:.cpp=.o)

CC		= clang++

CPPFLAGS	= -Wall -Wextra -Werror -std=c++98

all:	$(NAME)

$(NAME):	$(OBJECTS)
			@echo "\n	      ##### Compilando WebServer 🍋 #####\n";
			$(CC) $(CPPFLAGS) $(OBJECTS) -o $(NAME)

clean:
			@rm -rf $(OBJECTS)
			@echo "\n	       ##### Objects Cleaned!! #####\n";

fclean:	
			@rm -rf $(NAME) $(OBJECTS)
			clear
			@echo "\n	      ##### All files cleaned!! #####\n";
			

re:			fclean all
			
.PHONY:		all bonus clean fclean re