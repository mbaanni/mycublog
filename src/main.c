# include "../includes/Cub3D.h"
#include <stdbool.h>

int parse_map(t_mlx *mlx)
{
    int i;
    int j;
    i = 0;
    while (mlx->map[i])
    {
        j = 0;
        while (mlx->map[i][j])
        {
            if (mlx->map[i][j] == 'P')
            {
                mlx->map[i][j] = '0';
                mlx->movex = (j * upscale_map);
                mlx->movey = (i * upscale_map);
                mlx->angle = PI/2;
            }
            j++;
        }
        i++;
    }

    mlx->map_hight = i*upscale_map;
    mlx->map_width = j*upscale_map;
    return (0);
}
int main(int ac, char **av)
{
    t_mlx mlx;

    if (ac < 2)
        return 0;
    mlx.x = 0;
    mlx.y = 0;
    mlx.map = open_map(av[1]);
    parse_map(&mlx);
    mlx.mlx = mlx_init(1920, 1080, "MLX42", false);
    mlx.img = mlx_new_image(mlx.mlx, map_w, map_h);
    mlx.minimap_img = mlx_new_image(mlx.mlx, mini_map_w, mini_map_h);
    mlx_image_to_window(mlx.mlx, mlx.img, 0, 0);
    mlx_image_to_window(mlx.mlx, mlx.minimap_img, 0, 0);
    mlx.tile = mlx_load_png("image/tile.png");
    if(!mlx.tile)
        exit(0);
    mlx.start = 1;
    mlx_key_hook(mlx.mlx, event_win, &mlx);
    mlx_loop_hook(mlx.mlx, drow_player, &mlx);
    mlx_loop(mlx.mlx);
    mlx_terminate(mlx.mlx);
}
