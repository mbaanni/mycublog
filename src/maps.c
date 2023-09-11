# include "../includes/Cub3D.h"

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
    j = 0;
    while (mlx->map[i][j] && mlx->map[i][j] != '\n')
    {
        if (mlx->map[i][j] == '1')
            put_pixel(0xffffffff, mlx, j*upscale_map, i*upscale_map);
        if (mlx->map[i][j] == '0')
            put_pixel(0x000000ff, mlx, j*upscale_map, i*upscale_map);
        j++;
		if ((!mlx->map[i][j] || mlx->map[i][j] == '\n') && mlx->map[i+1])
		{
			i++;
			j = 0;
		}
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

uint32_t get_color(t_mlx *mlx,float y, float x)
{
	int pos = (y * mlx->tile[mlx->side]->width + x) * mlx->tile[mlx->side]->bytes_per_pixel;
	if (pos < 0 || pos > (mlx->tile[mlx->side]->height * (mlx->tile[mlx->side]->width*4)) - 4)
        return 0;
	return (mlx->tile[mlx->side]->pixels[pos]<<24 | mlx->tile[mlx->side]->pixels[pos+1]<<16 | mlx->tile[mlx->side]->pixels[pos+2]<<8 | mlx->tile[mlx->side]->pixels[pos+3]);
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
	int texter_x = fmod(mlx->offset, upscale_map) * ((float)mlx->tile[mlx->side]->width / upscale_map);
	float y = wall_start;
	while (wall_start < wall_end && wall_start < map_h)
	{
		texter_y = (1.0-(wall_end - wall_start)/wall_strip_hight)*mlx->tile[mlx->side]->width;
		color = get_color(mlx,texter_y, texter_x);
		mlx_put_pixel(mlx->img, r, wall_start, color);
		wall_start++;
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

void    ffps(void *ptr, mlx_image_t *txt)
{
    mlx_t *mlx = (mlx_t*)ptr;
    mlx_put_pixel(txt, 0, 0, 0xff0000ff);
}

void    drow_player(void *ptr)
{
	t_mlx *mlx;
    int fps;
	mlx = (t_mlx*)ptr;
	if (mlx->start)
    {
        fps = mlx->mlx->delta_time*1000;
        if (mlx->txt)
        {
            mlx_delete_image(mlx->mlx, mlx->txt);
            mlx->txt = 0;
        }
        mlx->txt = mlx_put_string(mlx->mlx, ft_itoa(fps),map_w/2,0);
        if (mlx->txt)
            ffps(mlx->mlx,mlx->txt);
		drow_map(mlx);
		draw_ceiling_floor(mlx);
		draw_ray(mlx);
		mlx->start = 0;
	}
}
