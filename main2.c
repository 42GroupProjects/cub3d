/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 15:19:37 by lwittwer          #+#    #+#             */
/*   Updated: 2026/06/21 14:51:45 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

void	draw_square(t_img *img, int x, int y, int size, int color)
{
	for (int y0 = 0; y0 < size; y0++)
	{
		for (int x0 = 0; x0 < size; x0++)
			put_pixel(img, x + x0, y + y0, color);
	}
}

void	draw_map(t_img *img, t_player *p)
{
	for (int y = 0; map[y]; y++)
	{
		for (int x = 0; map[y][x]; x++)
		{
			if (map[y][x] == '1')
				draw_square(img, x * TILE, y * TILE, TILE, 0xFF0000);
			else if (map[y][x] == '0')
				draw_square(img, x * TILE, y * TILE, TILE, 0x000000);
			else if (map[y][x] == 'P')
			{
				p->x = x * TILE + TILE / 2;
				p->y = y * TILE + TILE / 2;
				draw_square(img, x * TILE, y * TILE, TILE, 0xFFFF00);
			}
		}
	}
}

void	draw_player(t_img *img, t_player *p)
{
	draw_square(img, p->x - 3, p->y - 3, 6, 0x00FFFF);
}

void	draw_direction(t_img *img, t_player *p)
{
	float	dir_x;
	float	dir_y;

	dir_x = cos(p->angle);
	dir_y = sin(p->angle);
	for (int i = 0; i < 30; i++)
		put_pixel(img, p->x + dir_x * i, p->y + dir_y * i, 0x00FF00);
}

void	cast_ray(t_img *img, t_player *p)
{
	float	ray_x;
	float	ray_y;
	float	dir_x;
	float	dir_y;

	ray_x = p->x;
	ray_y = p->y;
	dir_x = cos(p->angle);
	dir_y = sin(p->angle);
	while (1)
	{
		ray_x += dir_x;
		ray_y += dir_y;
		int	map_x = ray_x / TILE;
		int	map_y = ray_y / TILE;
		if (map[map_y][map_x] == '1')
			break;
		put_pixel(img, ray_x, ray_y, 0xFFFFFF);
	}
}

int	key_hook(int keycode, t_player *p)
{
//	(void)p;
//	printf("keycode = %d\n", keycode);
	if (keycode == ARROW_LEFT)
		p->angle -= 0.1;
	if (keycode == ARROW_RIGHT)
		p->angle += 0.1;
	printf("%f\n", p->angle);
	return (0);
}
int	main(void)
{
	void	*mlx;
	void	*win;
	t_img	img;
	t_player	player;

	player.angle = 0;
	mlx = mlx_init();
	win = mlx_new_window(mlx, 400, 400, "Raycaster");
	img.img = mlx_new_image(mlx, 400, 400);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
	mlx_key_hook(win, key_hook, &player);
	draw_map(&img, &player);
	cast_ray(&img, &player);
	draw_player(&img, &player);
	//draw_direction(&img, &player);
	mlx_put_image_to_window(mlx, win, img.img, 0, 0);
	mlx_loop(mlx);
}
