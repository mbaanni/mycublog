/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaanni <mbaanni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 10:00:26 by mbaanni           #+#    #+#             */
/*   Updated: 2023/09/10 18:29:33 by mbaanni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cub3D.h"
#include <stdio.h>

void    clean_it(t_mlx *mlx)
{
    int i;
    int j;
    i = 0;
    while (i < 1080)
    {
        j = 0;
        while (j < 1920)
        {
            mlx_put_pixel(mlx->img, j, i, 0xff000000);
            mlx_put_pixel(mlx->minimap_img, j, i, 0xff000000);
            j++;
        }
		i++;
    }
}

void    check_next_xy(t_mlx *mlx, int x, int y)
{
    if ((mlx->map[(int)((mlx->movey + y)/upscale_map)][(int)(mlx->movex/upscale_map)]) != '1')
		mlx->movey += y;
	if (mlx->map[(int)(mlx->movey/upscale_map)][(int)((mlx->movex + x) /upscale_map)] != '1')
		mlx->movex += x;
}

void    player_movement(t_mlx *mlx)
{
    float x;
    float y;
    if (mlx_is_key_down(mlx->mlx, MLX_KEY_W))
        check_next_xy(mlx, cos(mlx->angle) * PLAYER_SPEED,
			sin(mlx->angle) * PLAYER_SPEED);
    else if (mlx_is_key_down(mlx->mlx, MLX_KEY_S))
		check_next_xy(mlx, -cos(mlx->angle) * PLAYER_SPEED,
			-sin(mlx->angle)*PLAYER_SPEED);
    else if (mlx_is_key_down(mlx->mlx, MLX_KEY_D))
		check_next_xy(mlx, cos(mlx->angle+PI/2)*PLAYER_SPEED,
			sin(mlx->angle+PI/2)*PLAYER_SPEED);
    else if (mlx_is_key_down(mlx->mlx, MLX_KEY_A))
		check_next_xy(mlx, -cos(mlx->angle+PI/2)*PLAYER_SPEED,
			-sin(mlx->angle+PI/2)*PLAYER_SPEED);
}

void	player_angle(t_mlx *mlx)
{
	if (mlx_is_key_down(mlx->mlx, MLX_KEY_RIGHT))
    {
        mlx->angle += speed;
        if (mlx->angle >= 2*PI)
            mlx->angle -= 2*PI;
    }
    else if (mlx_is_key_down(mlx->mlx, MLX_KEY_LEFT))
    {
        mlx->angle -= speed;
        if (mlx->angle < 0)
            mlx->angle += 2*PI;
    }
}

void event_win(void *param)
{
    t_mlx *mlx;

    mlx = (t_mlx *)param;
	mlx->start = 1;
    if (mlx_is_key_down(mlx->mlx, MLX_KEY_ESCAPE))
    {
        mlx_close_window(mlx->mlx);
        exit(0);
    }
    player_movement(mlx);
    player_angle(mlx);
}
