/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaanni <mbaanni@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 12:25:22 by mbaanni           #+#    #+#             */
/*   Updated: 2023/09/14 19:35:54 by mbaanni          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cub3D.h"

void	small_dist(t_ray *ray, t_mlx *mlx, float *distray)
{
    double  v_dist_sqr;
    double  h_dist_sqr;
    v_dist_sqr = ((ray->vx - mlx->movex) * (ray->vx - mlx->movex)) + ((ray->vy - mlx->movey) * (ray->vy - mlx->movey));
	h_dist_sqr = ((ray->hx - mlx->movex) * (ray->hx - mlx->movex)) + ((ray->hy - mlx->movey) * (ray->hy - mlx->movey));
    if (h_dist_sqr > v_dist_sqr)
    {
        ray->rx = ray->vx;
        ray->ry = ray->vy;
        *distray = sqrt(v_dist_sqr);
        mlx->offset = fmod(ray->vy, upscale_map);
        mlx->side = RIGHT;
        if (ray->vxblock > 0)
            mlx->side = LEFT;
    }
    else {
        ray->rx = ray->hx;
        ray->ry = ray->hy;
        *distray = sqrt(h_dist_sqr);
        mlx->offset = fmod(ray->hx, upscale_map);
        mlx->side = TOP;
        if (ray->hyblock < 0)
            mlx->side = BOTTOM;
    }
}

void    calculate_horizontal(float ra, t_mlx *mlx, t_ray *ray)
{
    float   tang;
    tang = -tan(ra);
    if (ra > PI)
    {
        ray->hy = ((mlx->movey / 64 ) * 64 ) - 0.0001;
        ray->hx = mlx->movex + (mlx->movey - ray->hy)/tang;
        ray->hyblock = -64;
        ray->hxblock = -ray->hyblock/tang;
    }
    if(ra < PI)
    {
        ray->hy = ((mlx->movey / 64) * 64) + 64;
        ray->hx = mlx->movex + (mlx->movey - ray->hy)/tang;
        ray->hyblock = 64;
        ray->hxblock = -ray->hyblock/tang;
    }
    //next x
    while(1)
    {
        if (ray->hx > mlx->map_width || ray->hx < 0)
            break ;
        if (mlx->map[(int)ray->hy/64][(int)ray->hx/64] == '1')
            break ;
        ray->hx = ray->hx + ray->hxblock;
        ray->hy = ray->hy + ray->hyblock;
    }
}

void    calculate_vertical(float ra, t_mlx *mlx, t_ray *ray)
{
    float   tang;

    tang = -tan(ra);
    if (ra > (3*PI)/2 ||  ra < PI/2)
    {
        ray->vx = (( mlx->movex / 64 ) * 64 ) + 64;
        ray->vy = mlx->movey + (mlx->movex - ray->vx) * tang;
        ray->vxblock = 64;
        ray->vyblock = -ray->vxblock*tang;
    }
    if (ra > (PI/2) && ra < (3*PI/2))
    {
        ray->vx = ((mlx->movex / 64 ) * 64 ) - 0.0001;
        ray->vy = mlx->movey + (mlx->movex - ray->vx) * tang;
        ray->vxblock = -64;
        ray->vyblock = -ray->vxblock*tang;
    }
    while (ray->vx < mlx->map_width && ray->vx > 0)
    {
        if (ray->vy > mlx->map_hight || ray->vy < 0)
            break ;
        if (mlx->map[(int)ray->vy/64][(int)ray->vx/64] == '1')
			break ;
		ray->vx = ray->vx + ray->vxblock;
		ray->vy = ray->vy + ray->vyblock;
    }
}