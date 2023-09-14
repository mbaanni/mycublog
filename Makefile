
OS = $(shell uname)
LIBMLX = ./42mx
ifeq ($(OS),Linux)
	LIBS = $(LIBMLX)/build/libmlx42.a -Iinclude -ldl -lglfw -pthread -lm
else
	LIBS = $(LIBMLX)/build/libmlx42.a -ldl -lglfw -L"$(HOME)/.brew/Cellar/glfw/3.3.8/lib" -pthread -lm -framework Cocoa -framework OpenGL -framework IOKit
endif
CFLAG= -g -fsanitize=address #-Wall -Wextra -Werror
HEADER = -I ./includes -I $(LIBMLX)/include
CC= cc
SRC= main.c event.c maps.c reading_map.c calculation.c draw_line.c draw_mini_map.c
NAME= Cub3d
DIR_SRC = src/
DIR_OBG = obj/
OBG= $(addprefix $(DIR_OBG), $(SRC:.c=.o))

all : libmlx $(NAME)

libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build
	@make -C $(LIBMLX)/build/.

$(NAME) : $(OBG)
	$(CC) $(CFLAG) $(OBG) $(LIBS) libft/libft.a $(HEADER) -o $(NAME)

$(DIR_OBG)%.o: $(DIR_SRC)%.c includes/Cub3D.h
	@mkdir -p $(DIR_OBG)
	@make -C libft > /dev/null
	@$(CC) $(CFLAG) $(HEADER) -c $< -o $@
clean :
	@make -C libft clean
	@rm -rf $(DIR_OBG)
fclean : clean
	@make -C libft fclean
	@rm -rf $(NAME)
re : fclean all

.PHONY: all clean fclean re




