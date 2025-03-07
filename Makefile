##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile for the MyWorld project.
##

NAME	=	my_world

SRC	=	$(shell find src/ -type f -name '*.c')

LIBS	=	-l csfml-graphics -l csfml-system -l csfml-audio	\
		-l csfml-window -lm

OBJ	=	$(SRC:.c=.o)

all	:	$(NAME)

$(NAME)	:	$(OBJ)
		gcc -o $(NAME) $(OBJ) $(LIBS)

clean	:
		rm -rf $(OBJ)

fclean	:	clean
		rm -rf $(NAME)

re	:	fclean all
