# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/25 14:46:34 by fyuzhyk           #+#    #+#              #
#    Updated: 2022/10/09 16:20:21 by fyuzhyk          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fdf

LIBFT_DIR = lib/libft
MLX_DIR = lib/MLX42
SRC_DIR = src
LIB = fdf.a

LIBFT = $(LIBFT_DIR)/libft.a
MLX = $(MLX_DIR)/libmlx42.a

CFLAGS = -Wall -Wextra -Werror

SRC = src/bresenham.c src/create_frame.c src/draw.c src/free.c src/abs.c src/hooks.c src/init_mlx.c src/init_values.c src/input_parsing.c\
src/main.c src/move_zoom_hooks.c src/node_management.c src/node_utils.c src/operations.c src/rotations.c src/utils.c src/z_manipulation.c\
src/draw_decoration.c

OBJ = $(SRC:%.c=%.o)

all : $(NAME)

$(NAME) : $(OBJ) $(LIB)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) $(LIBFT) $(MLX) -I include -lglfw -L "/Users/$(USER)/.brew/opt/glfw/lib/" -o $(NAME)

$(LIB) : $(OBJ)
	Make -C $(LIBFT_DIR)
	Make -C $(MLX_DIR)
	ar rc $(LIB) $(OBJ)

%.o: %.c
	$(CC) -c $(CFLAGS) -Imlx -c $< -o $@

fclean: clean
	rm -f $(NAME) $(LIBFT_DIR)/*.o $(LIBFT_DIR)/libft.a $(MLX_DIR)/libmlx42.a $(LIB)

clean:
	rm -f $(OBJ)

re: fclean all
