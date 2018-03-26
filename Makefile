# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/05/20 17:57:50 by frmarinh          #+#    #+#              #
#    Updated: 2017/09/18 04:57:47 by frmarinh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME_PESTILENCE	=	Pestilence

NAME		=   Pestilence
NAMEBASE    =   $(shell basename $(NAME))
LENGTHNAME	=	`printf "%s" $(NAMEBASE) | wc -c`
MAX_COLS	=	$$(echo "$$(tput cols)-24-$(LENGTHNAME)"|bc)

CC			=	gcc -lssl -lcrypto -ldl -lpthread
FLAGS_O		=

SRCDIR_PESTILENCE		=	srcs/
OBJDIR_PESTILENCE		=	objs/

INCDIR				=	includes/

SRCBASE_PESTILENCE	=	\
					main.c				\
					aes.c				\
					key_iv.c			\
					cipher_plain.c		\
					infection.c			\
					safe.c				\
					tracer.c			\
					process_exists.c	\
					persistent_process.c

INCS			=	$(addprefix $(INCDIR), $(INCBASE))

SRCS_PESTILENCE			=	$(addprefix $(SRCDIR_PESTILENCE), $(SRCBASE_PESTILENCE))
OBJS_PESTILENCE			=	$(addprefix $(OBJDIR_PESTILENCE), $(SRCBASE_PESTILENCE:.c=.o))

.SILENT:

all:		$(NAME)

$(NAME):
	if test -f $(NAME_PESTILENCE) ; then											\
		echo "make: Nothing to be done for \`all\`.";				        		\
	else																			\
		make PESTILENCE && 														\
		echo "\r\033[38;5;184m👥  AUTHOR(s): \033[0m\033[K" && 						\
		echo "\r\033[38;5;15m`cat auteur | sed s/^/\ \ \ \ -/g`\033[0m\033[K" &&	\
		echo "\033[38;5;44m☑️  ALL    $(NAMEBASE) is done\033[0m\033[K"	  &&	 	\
		strip $(NAME_PESTILENCE);													\
	fi

$(OBJDIR_PESTILENCE):
	mkdir -p $(OBJDIR_PESTILENCE)
	mkdir -p $(dir $(OBJS_PESTILENCE))

$(OBJDIR_PESTILENCE)%.o : $(SRCDIR_PESTILENCE)%.c | $(OBJDIR_PESTILENCE)
	$(CC) $(FLAGS) -c $< -o $@											\
		-I $(INCDIR)														\
		-I $(INCDIR)

fcleanlib:	fclean

re:			fclean all

relib:		fclean fcleanlib all

.PHONY:		fclean fcleanlib clean re relib

PESTILENCE: $(OBJDIR_PESTILENCE) $(OBJS_PESTILENCE)
	$(CC) $(FLAGS) $(FLAGS_O) -o $(NAME_PESTILENCE) $(OBJS_PESTILENCE)
	echo "\r\033[38;5;22m📗  MAKE $(NAME_PESTILENCE)"

clean:
	@rm -rf $(OBJDIR_PESTILENCE)
	echo "\r\033[38;5;124m📕  CLEAN $(OBJDIR_PESTILENCE)\033[0m\033[K";

fclean:		clean
	@rm -rf $(NAME_PESTILENCE)
	echo "\r\033[38;5;124m📕  FCLEAN $(NAME_PESTILENCE)\033[0m\033[K";

-include $(OBJS:.o=.d)
