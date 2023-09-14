#ifndef CUB3D_H
#define CUB3D_H

#include "../42mx/include/MLX42/MLX42.h"
#include "../libft/libft.h"
#include "../libft/get_next_line.h"
#include <fcntl.h>
#include <stdio.h> //remove this library
#include <math.h>

#define PI M_PI
#define map_w 1920
#define map_h 1080
#define mini_map_w 1920
#define mini_map_h 1080
#define upscale_map 64
#define map_size 0.2
#define angle_speed 0.05
#define PLAYER_SPEED 15
#define field_of_view 60

enum prespective {
    NO,
    SO,
    WE,
    EA
};

enum
{
    TOP,
    BOTTOM,
    RIGHT,
    LEFT,
};

typedef struct t_ray {
    float   hx;
    float   hy;
    float   rx;
    float   ry;
    float   vx;
    float   vy;
    float   hxblock;
    float   hyblock;
    float   vxblock;
    float   vyblock;
}t_ray;

typedef struct t_mlx {
    mlx_image_t *img;
    mlx_image_t *minimap_img;
    mlx_texture_t* tile[5];
    t_ray   *ray;
    mlx_t*  mlx;
    mlx_image_t *txt;
    int     side;
    void    *mlx_win;
    void    *xpm;
    int start;
    float offset;
    int map_hight;
    int map_width;
    float angle;
    int movex;
    int movey;
    int color;
    char **map;
}   t_mlx;

int     drow_map(t_mlx *mlx);
void    clean_it(t_mlx *mlx);
int     parse_map(t_mlx *mlx);
void    drow_player(void *mlx);
void    event_win(void *param);
char    **open_map(char *link);
int     creat_block(t_mlx *mlx);
int     drow_map(t_mlx *mlx);
void    draw_line(mlx_image_t *image, int startx, int starty, int endx, int endy, int color);
void    calculate_vertical(float ra, t_mlx *mlx, t_ray *ray);
void    calculate_horizontal(float ra, t_mlx *mlx, t_ray *ray);
void	small_dist(t_ray *ray, t_mlx *mlx, float *distray);

#endif