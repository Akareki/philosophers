# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aoizel <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/21 13:05:56 by aoizel            #+#    #+#              #
#    Updated: 2023/12/22 08:14:58 by aoizel           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	philo

INCLUDES		=	-I. 

LINKS			=	-lpthread

CFLAGS 			=	-Wall -Wextra -Werror

CC 				=	cc $(CFLAGS)

SOURCES 		=	philo.c init.c philo_life.c philo_utils.c

OBJECTS 		=	$(addprefix $(OBJECTS_DIR),$(SOURCES:.c=.o))

OBJECTS_DIR		=	.objs/

DEPENDENCIES	=	Makefile

MAKE			=	@make --no-print-directory


all:		$(OBJECTS_DIR) $(NAME) 

$(NAME):	$(OBJECTS) | $(OBJECTS_DIR) 
				$(CC) $(OBJECTS) $(LINKS) $(INCLUDES) -o $(NAME)

$(OBJECTS_DIR):
				mkdir -p .objs


$(OBJECTS_DIR)%.o: %.c $(DEPENDENCIES)
				$(CC) -c $(INCLUDES) $< -o $@

$(OBJECTS_DIR)%.d: %.c | $(OBJECTS_DIR)
				$(CC) $< -MT $(OBJECTS_DIR)$(<:.c=.o) -MM -MP -MF $@

clean:
				rm -rf $(OBJECTS_DIR)

fclean:		clean
				rm -rf $(NAME)

re:			fclean 
				make all

.PHONY: 	all clean fclean re 

-include $(OBJECTS:.o=.d)
