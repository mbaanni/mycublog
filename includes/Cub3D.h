#ifndef CUB3D_H
#define CUB3D_H

#include "../42mx/include/MLX42/MLX42.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../libft/libft.h"
#include "../libft/get_next_line.h"

#define PI M_PI
#define map_w 1920
#define map_h 1080
#define mini_map_w 1920
#define mini_map_h 1080
#define upscale_map 64
#define upscale_player 12
#define map_size 0.2
#define speed 0.05
#define PLAYER_SPEED 5
#define field_of_view 60

enum
{
    TOP,
    BOTTOM,
    RIGHT,
    LEFT,
};

typedef struct t_ray {
    int     side;
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
    float   offset;
}t_ray;

typedef struct t_mlx {
    mlx_image_t *img;
    mlx_image_t *minimap_img;
    mlx_texture_t* tile;
    t_ray   *ray;
    mlx_t* mlx;
    void *mlx_win;
    void *xpm;
    int x;
    int start;
    int y;
    int map_hight;
    int map_width;
    float angle;
    int movex;
    int movey;
    int rx;
    float ry;
    int color;
    char **map;
}   t_mlx;

void event_win(mlx_key_data_t key, void *param);
char **open_map(char *link);
int    creat_block(t_mlx *mlx);
int drow_map(t_mlx *mlx);
void    draw_line(mlx_image_t *image, int startx, int starty, int endx, int endy, int color);
void    drow_player(void *mlx);
void    clean_it(t_mlx *mlx);

enum prespective {
    NO,
    SO,
    WE,
    EA
};
#endif