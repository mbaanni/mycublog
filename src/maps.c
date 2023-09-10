# include "../includes/Cub3D.h"
#include <_types/_uint32_t.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void    put_pixel(int color, t_mlx *mlx, int x, int y)
{
    int i;
    int j;

    i = 0;
    while (i < upscale_map)
    {
        j = 0;
        while (j < upscale_map)
        {
			if (j != 0 && i != 0)
            	mlx_put_pixel(mlx->minimap_img, (x + j)*map_size, (y + i)*map_size, color);
            j++;
        }
        i++;
    }
}
int drow_map(t_mlx *mlx)
{
    int i;
    int j;
    i = 0;
    while (mlx->map[i])
    {
        j = 0;
        while (mlx->map[i][j] && mlx->map[i][j] != '\n')
        {
            if (mlx->map[i][j] == '1')
                put_pixel(0xffffffff, mlx, j*upscale_map, i*upscale_map);
            if (mlx->map[i][j] == '0')
                put_pixel(0x000000ff, mlx, j*upscale_map, i*upscale_map);
            j++;
        }
        i++;
    }
    return (0);
}

void    draw_line(mlx_image_t *image, int startx, int starty, int endx, int endy, int color)
{
	int	delta_x = endx - startx;
	int	delta_y = endy - starty;
    int step_x = 1;
    int step_y = 1;
    if (delta_x < 0)
	{
		delta_x = -delta_x;
        step_x = -1;
	}
    if (delta_y < 0)
	{
		delta_y = -delta_y;
        step_y = -1;
	}
	delta_y*=-1;
    int error;
    int error2 = 0;
    error = delta_y+delta_x;
    while (1)
    {
        if (startx >= 0 && startx < map_w && starty >= 0 && starty < map_h)
    		mlx_put_pixel(image, startx, starty, color);
		if (startx == endx && starty == endy)
			break;
		error2 = error*2;
		if (error2 >= delta_y)
		{
			if (startx == endx)
				break;
			error+=delta_y;
			startx+=step_x;
		}
		if (error2 <= delta_x)
		{
			if (starty == endy)
				break;
			error+=delta_x;
			starty+=step_y;
		}
	}
}

float dist(float px, float py, float rx, float ry)
{
    float dx;
    float dy;

    dx = rx - px;
    dy = ry - py;

    float res = sqrt((dx*dx) + (dy*dy));
    return res;
}

void    draw_ceiling_floor(t_mlx *mlx)
{
    int color;
    int x,y;
    y = -1;
    color = 0x45b3e0ff;
    while (++y < map_h)
    {
        if (y > map_h/2)
            color = 0xffffff00;
        x = -1;
        while (++x < map_w)
            mlx_put_pixel(mlx->img, x, y, color);
    }
}

float   bound_angle(float angle)
{
    if (angle < 0)
        angle += 2 * PI;
    else if (angle >= 2 * PI)
        angle -= 2 * PI;
    return (angle);
}

void	small_dist(t_ray *ray, t_mlx *mlx, float *distray)
{
    double  v_dist_sqr;
    double  h_dist_sqr;
    double block = 64;
    v_dist_sqr = ((ray->vx - mlx->movex) * (ray->vx - mlx->movex)) + ((ray->vy - mlx->movey) * (ray->vy - mlx->movey));
	h_dist_sqr = ((ray->hx - mlx->movex) * (ray->hx - mlx->movex)) + ((ray->hy - mlx->movey) * (ray->hy - mlx->movey));
    if (h_dist_sqr > v_dist_sqr)
    {
        ray->rx = ray->vx;
        ray->ry = ray->vy;
        *distray = sqrt(v_dist_sqr);
        mlx->offset = ray->vy;
        ray->side = RIGHT;
        if (ray->vxblock > 0)
            ray->side = LEFT;
    }
    else {
        ray->rx = ray->hx;
        ray->ry = ray->hy;
        *distray = sqrt(h_dist_sqr);
        mlx->offset = ray->hx;
        ray->side = TOP;
        if (ray->hyblock < 0)
            ray->side = BOTTOM;
    }
}

void    calculate_horizontal(float ra, t_mlx *mlx, t_ray *ray)
{
    float   atan;

    atan = -tan(ra);
    if (ra > PI)
    {
        ray->hy = ((mlx->movey / 64 ) * 64 ) - 0.0001;
        ray->hx = mlx->movex + (mlx->movey - ray->hy)/atan;
        ray->hyblock = -64;
        ray->hxblock = -ray->hyblock/atan;
    }
    if(ra < PI)
    {
        ray->hy = ((mlx->movey / 64) * 64) + 64;
        ray->hx = mlx->movex + (mlx->movey - ray->hy)/atan;
        ray->hyblock = 64;
        ray->hxblock = -ray->hyblock/atan;
    }
    //next x
    while(ray->hy < mlx->map_hight && ray->hy > 0)
    {
        if (ray->hx > mlx->map_width || ray->hx < 0)
            break ;
        else if (mlx->map[(int)ray->hy/64][(int)ray->hx/64] == '1')
            break ;
        ray->hx = ray->hx + ray->hxblock;
        ray->hy = ray->hy + ray->hyblock;
    }
}

void    calculate_vertical(float ra, t_mlx *mlx, t_ray *ray)
{
    float   atan;
    atan = -tan(ra);
    if (ra > (3*PI)/2 ||  ra < PI/2)
    {
        ray->vx = (( mlx->movex / 64 ) * 64 ) + 64;
        ray->vy = mlx->movey + (mlx->movex - ray->vx) * atan;
        ray->vxblock = 64;
        ray->vyblock = -ray->vxblock*atan;
    }
    if (ra > (PI/2) && ra < (3*PI/2))
    {
        ray->vx = ((mlx->movex / 64 ) * 64 ) - 0.0001;
        ray->vy = mlx->movey + (mlx->movex - ray->vx) * atan;
        ray->vxblock = -64;
        ray->vyblock = -ray->vxblock*atan;
    }
    while (ray->vx < mlx->map_width && ray->vx > 0)
    {
        if (ray->vy > mlx->map_hight || ray->vy < 0)
            break ;
        else if (mlx->map[(int)ray->vy/64][(int)ray->vx/64] == '1')
            break ;
        ray->vx = ray->vx + ray->vxblock;
        ray->vy = ray->vy + ray->vyblock;
    }
}

uint32_t get_color(t_mlx *mlx,float y, float x)
{
	int pos = (y * mlx->tile->width + x) * mlx->tile->bytes_per_pixel;
	if (pos < 0 || pos > (mlx->tile->height * (mlx->tile->width*4)) - 4)
        return 0;
	return (mlx->tile->pixels[pos]<<24 | mlx->tile->pixels[pos+1]<<16 | mlx->tile->pixels[pos+2]<<8 | mlx->tile->pixels[pos+3]);
}
void    draw_wall(t_mlx *mlx, t_ray *ray, int r, float distray, float angle_step)
{
    float wall_strip_hight;
    if (distray < 4)
        distray = 4;
    // //fish eyes
    distray = cos(30*PI/180-(r*angle_step))*distray;
    wall_strip_hight = ((float)64/distray)*(((float)map_h/2)/(tan(30*PI/180)));
    float	wall_start = map_h/2 - (wall_strip_hight/2);
	float	wall_end = wall_strip_hight + wall_start;
	if (wall_start < 0)
		wall_start = 0;
	uint32_t color;
	int texter_y;
	int texter_x = fmod(mlx->offset, upscale_map) * ((float)mlx->tile->width / upscale_map);
	float y = wall_start;
	while (y < wall_end && y < map_h)
	{
		texter_y = ((wall_end - y)/wall_strip_hight)*mlx->tile->width;
		color = get_color(mlx,texter_y, texter_x);
		mlx_put_pixel(mlx->img, r, y, color);
		y++;
    }
}

void draw_ray(t_mlx *mlx)
{
    t_ray	ray;
    int		r;
    float	ra;
	float	angle_step;
    float	distray;

    r = -1;
    ra = mlx->angle - ((float)field_of_view / 2) * (PI/180);
    angle_step = (field_of_view * (PI/180)) / map_w;
    while(++r < map_w)
    {
		ra = bound_angle(ra);
		calculate_horizontal(ra, mlx, &ray);
		calculate_vertical(ra, mlx, &ray);
		small_dist(&ray, mlx, &distray);
		draw_line(mlx->minimap_img, (mlx->movex)*map_size, (mlx->movey)*map_size, (ray.rx)*map_size, (ray.ry)*map_size,0x00FF00FF);
		draw_wall(mlx, &ray, r, distray, angle_step);
		ra += angle_step;
    }
}

void    drow_player(void *ptr)
{
    t_mlx *mlx;
    mlx = (t_mlx*)ptr;
    if (mlx->start)
    {
        //clean_it(mlx);// remove this function
        drow_map(mlx);
        draw_ceiling_floor(mlx);
	    draw_ray(mlx);
        mlx->start = 0;
    }
}

char **read_it(int fd)
{
    char	*line;
	char	*str;
	char	**map;

	str = 0;
    map = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		str = ft_strjoin(str, line);
		str = ft_strjoin(str, " ");
		free(line);
	}
	map = ft_split(str, ' ');
	free(str);
	close(fd);
	if (!map)
		write(1, "not allocated\n", 14);
	return (map);
}

char **open_map(char *link)
{
    int fd;
    fd = open(link, O_RDONLY);
    if (fd < 0)
        return (0);
    return (read_it(fd));
}
