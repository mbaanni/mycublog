/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaanni <mbaanni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 10:00:26 by mbaanni           #+#    #+#             */
/*   Updated: 2023/09/14 19:32:01 by mbaanni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cub3D.h"

void    check_next_xy(t_mlx *mlx, int x, int y)
{
    int offsetx = 15;
    int offsety = 15;
    if (x < 0)
        offsetx = -offsetx;
    if (y < 0)
        offsety = -offsety;
    if ((mlx->map[(int)((mlx->movey + y + offsety)/upscale_map)][(int)(mlx->movex/upscale_map)]) != '1')
	{
        mlx->movey += y;
        mlx->start = 1;
	}
    if (mlx->map[(int)(mlx->movey/upscale_map)][(int)((mlx->movex + x + offsetx) /upscale_map)] != '1')
    {
        mlx->movex += x;
        mlx->start = 1;
    }
}

void    player_movement(t_mlx *mlx)
{
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
        mlx->angle += angle_speed;
        if (mlx->angle >= 2*PI)
            mlx->angle -= 2*PI;
	    mlx->start = 1;
    }
    else if (mlx_is_key_down(mlx->mlx, MLX_KEY_LEFT))
    {
        mlx->angle -= angle_speed;
        if (mlx->angle < 0)
            mlx->angle += 2*PI;
	    mlx->start = 1;
    }
}

void event_win(void *param)
{
    t_mlx *mlx;

    mlx = (t_mlx *)param;    
    if (mlx_is_key_down(mlx->mlx, MLX_KEY_ESCAPE))
    {
		mlx_close_window(mlx->mlx);
		exit(0);
    }
    player_movement(mlx);
    player_angle(mlx);
}
