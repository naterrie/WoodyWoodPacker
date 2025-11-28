# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: naterrie <naterrie@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/22 11:25:01 by ffaivre           #+#    #+#              #
#    Updated: 2025/11/28 16:33:24 by naterrie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

############################## Variables #######################################

NAME 			:=	woody_woodpacker

CC 				:= 	gcc
CFLAGS 			:= 	-Wall -Wextra -Werror -I $(DIR_INCS) -g3

DIR_SRCS		:=	srcs
DIR_OBJS		:=	.objs
DIR_INCS		:=	incs

LST_INCS		:=	woody.h

LST_SRCS 		:=	main.c \
					libft.c \
					parsing/parsing.c \
					parsing/woody.c \
					encrypt/xtea.c \
					stub/stub.c \
					stub/stub_bytes.c

LST_OBJS		:=	$(LST_SRCS:.c=.o)

SRCS			:=	$(addprefix $(DIR_SRCS)/,$(LST_SRCS))
OBJS			:=	$(addprefix $(DIR_OBJS)/,$(LST_OBJS))
INCS			:=	$(addprefix $(DIR_INCS)/,$(LST_INCS))

PRINTF = LC_NUMERIC="en_US.UTF-8" printf


############################## Rules ##########################################

all:	stub $(NAME)

$(NAME): $(OBJS) $(INCS)
	@$(CC) $(CFLAGS) $^ -o $@
	@$(PRINTF) "$(_PURPLE)$@ is up to date!$(_WHITE)\n"

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.c $(INCS) Makefile | $(DIR_OBJS)
	@mkdir -p $(dir $@)
	@$(PRINTF) "Compiling $(_BLUE)$<$(_WHITE) into $@...\n"
	@$(CC) $(CFLAGS) -I $(DIR_INCS) -c $< -o $@

$(DIR_OBJS):
	@mkdir -p $(DIR_OBJS)

clean:
	@$(PRINTF) "$(_GREEN)Cleaning up object files in $(DIR_OBJS)...$(_WHITE)\n"
	@rm -rf $(DIR_OBJS)
	@rm woody

fclean: clean
	@$(PRINTF) "$(_YELLOW)Removing $(NAME)$(_WHITE)\n"
	@rm -f $(NAME)

re: fclean all

git: fclean
	@$(PRINTF) "$(_RED)\nAdding GIT...$(_WHITE)\n"
	git status
	git add .
	@if [ "$(word 2,$(MAKECMDGOALS))" = "" ]; then \
		git commit -m "Auto-Commit" || echo "Nothing to commit"; \
	else \
		git commit -m "$(word 2,$(MAKECMDGOALS))" || echo "Nothing to commit"; \
	fi
	git push

stub:
	@gcc -c -fPIC -nostdlib -Os srcs/stub/stub.c -o srcs/stub/stub.o
	@objcopy -O binary srcs/stub/stub.o srcs/stub/stub.bin
	@xxd -i srcs/stub/stub.bin > srcs/stub/stub_bytes.c
	@rm srcs/stub/stub.o srcs/stub/stub.bin
	@echo "$(__GREEN)Stub compiled!$(_WHITE)"

.PHONY: all clean fclean re git


############################## Colors #########################################

_GREY		= \033[38m
_RED		= \033[91m
_GREEN		= \033[92m
_YELLOW		= \033[33m
_BLUE		= \033[34m
_PURPLE		= \033[35m
_CYAN		= \033[36m
_WHITE		= \033[37m
_MAGENTA 	= \033[95m
