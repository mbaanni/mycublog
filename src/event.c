/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaanni <mbaanni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 10:00:26 by mbaanni           #+#    #+#             */
/*   Updated: 2023/09/09 10:28:58 by mbaanni          ###   ########.fr       */
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

void event_win(mlx_key_data_t key, void *param)
{
    t_mlx *mlx;
    float x;
    float y;

    mlx = (t_mlx *)param;
	mlx->start = 1;
    if (key.key == MLX_KEY_ESCAPE)
    {
        mlx_close_window(mlx->mlx);
        exit(0);
    }
    else if (key.key == MLX_KEY_W && key.action == MLX_PRESS)
    {
        x = cos(mlx->angle)*PLAYER_SPEED;
        y = sin(mlx->angle)*PLAYER_SPEED;
        if ((mlx->map[(int)((mlx->movey + y)/upscale_map)][(int)(mlx->movex/upscale_map)]) != '1')
			mlx->movey += y;
		if (mlx->map[(int)(mlx->movey/upscale_map)][(int)((mlx->movex + x) /upscale_map)] != '1')
			mlx->movex += x;
    }
    else if (key.key == MLX_KEY_S)
    {
        x = cos(mlx->angle)*PLAYER_SPEED;
        y = sin(mlx->angle)*PLAYER_SPEED;
        if ((mlx->map[(int)((mlx->movey - y)/upscale_map)][(int)(mlx->movex/upscale_map)]) != '1')
			mlx->movey -= y;
		if (mlx->map[(int)(mlx->movey/upscale_map)][(int)((mlx->movex - x)/upscale_map)] != '1')
			mlx->movex -= x;
    }
    else if (key.key == MLX_KEY_D)
    {
        x = cos(mlx->angle+PI/2)*PLAYER_SPEED;
        y = sin(mlx->angle+PI/2)*PLAYER_SPEED;
        if ((mlx->map[(int)((mlx->movey + y)/upscale_map)][(int)(mlx->movex/upscale_map)]) != '1')
			mlx->movey += y;
		if (mlx->map[(int)(mlx->movey/upscale_map)][(int)((mlx->movex + x)/upscale_map)] != '1')
			mlx->movex += x;
    }
    else if (key.key == MLX_KEY_A)
    {
        x = cos(mlx->angle+PI/2)*PLAYER_SPEED;
        y = sin(mlx->angle+PI/2)*PLAYER_SPEED;
        if ((mlx->map[(int)((mlx->movey - y)/upscale_map)][(int)mlx->movex/upscale_map]) != '1')
			mlx->movey -= y;
		if (mlx->map[(int)mlx->movey/upscale_map][(int)((mlx->movex - x)/upscale_map)] != '1')
			mlx->movex -= x;
    }
    else if (key.key == MLX_KEY_RIGHT)
    {
        mlx->angle += speed;
        if (mlx->angle >= 2*PI)
            mlx->angle -= 2*PI;
    }
    else if (key.key == MLX_KEY_LEFT)
    {
        mlx->angle -= speed;
        if (mlx->angle < 0)
            mlx->angle += 2*PI;
    }
}