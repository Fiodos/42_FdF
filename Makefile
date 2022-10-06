# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/25 14:46:34 by fyuzhyk           #+#    #+#              #
#    Updated: 2022/10/06 13:55:10 by fyuzhyk          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = main

SRC_DIR = mlx

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(SRC_DIR)/%.o)

all: $(NAME)

# $(NAME): $(OBJ)
# 	$(CC) $(OBJ) libft/libft.a -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) MLX42/libmlx42.a libft/libft.a -I include -lglfw -L "/Users/$(USER)/.brew/opt/glfw/lib/" -o $(NAME)


%.o: %.c
	$(CC) -Wall -Wextra -Werror -Imlx -c $< -o $@

fclean: clean
	rm -f $(NAME)

clean:
	rm -f $(OBJ)

re: fclean all
