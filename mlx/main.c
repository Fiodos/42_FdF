/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 14:48:24 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/06/29 18:16:51 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "lib/fdf.h"
#include "../libft/libft.h"
#include <sys/stat.h>
#include <fcntl.h>

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	bresenham_x(t_data *img, int x, int y, int x2, int y2) // if slope < 1;
{
	int	dx;
	int	dy;
	int	p;

	dx = x2 - x;
	dy = y2 - y;
	p = (2 * dy) - dx;
	while (x <= x2)
	{
		my_mlx_pixel_put(img, x, y, 0x00FF0000);
		x++;
		if (p < 0)
			p = p + 2 * dy;
		else
		{
			p = p + (2 * dy) - (2 * dx);
			y++;
		}
	}
}

void	bresenham_y(t_data *img, int x, int y, int x2, int y2) // if slope > 1;
{
	int	dx;
	int	dy;
	int	p;

	dx = x2 - x;
	dy = y2 - y;
	p = (2 * dy) - dx;
	while (y <= y2)
	{
		my_mlx_pixel_put(img, x, y, 0x00FF0000);
		y++;
		if (p < 0)
			p = p + 2 * dy;
		else
		{
			p = p + (2 * dy) - (2 * dx);
			x++;
		}
	}
}

void	print_vertical(t_data *img, int x, int y, int y2) // if slope == 0;
{
	while (y + 10 < y2)
	{
		my_mlx_pixel_put(img, x, y, 0x00FF0000);
		y++;
	}
}

void	create_frame(t_data *img, int x, int y) // j && i (x10);
{
	int	origin_x;
	int	origin_y;
	int	dest_x;
	int	dest_y;
	int	i;

	origin_x = 180;
	origin_y = 180;
	dest_x = 180 + x;
	dest_y = 180 + y;
	i = 0;
	while (i < y) // y = (i * 10);
	{
		bresenham_x(img, origin_x, origin_y, dest_x, origin_y);
		origin_y = origin_y + 10;
		i = i + 10;
	}
	i = -10;
	origin_y = 180;
	while (i < x) // x = (j * 10);
	{
		print_vertical(img, origin_x, origin_y, dest_y);
		origin_x = origin_x + 10;
		i = i + 10;
	}
}

int	main(int argc, char **argv)
{
	int	i = 0; // y;
	int	j = 0; // x;
	int		fd = open(argv[argc - 1], O_RDONLY);
	char	*line;
	char	**ps;
	void	*mlx;
	void	*mlx_win;
	t_data	img;

	line = get_next_line(fd);
	ps = ft_split(line, ' ');
	while (ps[j])
		j++;
	while (line)
	{
		ft_printf("%s", line);
		line = get_next_line(fd);
		i++;
	}
	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1920, 1080, "FdF");
	img.img = mlx_new_image(mlx, 1920, 1080);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								&img.endian);
	int	origin_x = 180;
	int	origin_y = 180;
	my_mlx_pixel_put(&img, origin_x, origin_y, 0x00FF0000);
	int	dest_x = origin_x + 5;
	int	dest_y = origin_y + 5;
	bresenham_x(&img, origin_x, origin_y, dest_x, dest_y);
	dest_y = origin_y - 5;
	my_mlx_pixel_put(&img, dest_x, dest_y, 0x00FF0000);
	mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
}
