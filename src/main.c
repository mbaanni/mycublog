# include "../includes/Cub3D.h"
#include <stdbool.h>

void	load_mlx(t_mlx *mlx)
{
	mlx->mlx = mlx_init(1920, 1080, "MLX42", false);
    mlx->img = mlx_new_image(mlx->mlx, map_w, map_h);
    mlx->minimap_img = mlx_new_image(mlx->mlx, mini_map_w, mini_map_h);
    mlx_image_to_window(mlx->mlx, mlx->img, 0, 0);
    mlx_image_to_window(mlx->mlx, mlx->minimap_img, 0, 0);
}
int	load_image(t_mlx *mlx)
{
    mlx->tile[0] = mlx_load_png("image/eagle.png");
    mlx->tile[1] = mlx_load_png("image/greystone.png");
    mlx->tile[2] = mlx_load_png("image/redbrick.png");
    mlx->tile[3] = mlx_load_png("image/bluestone.png");
	int i = -1;
	while (++i < 4)
	{
		if (!mlx->tile[i])
        {
            i--;
            while (i >= 0)
            {
                free (mlx->tile[i]);
                i--;
            }
			return (1);
        }
	}
	return (0);
}

int main(int ac, char **av)
{
    t_mlx mlx;

    if (ac != 2)
        return 1;
	ft_memset(&mlx, 0, sizeof(mlx));
    mlx.map = open_map(av[1]);
    parse_map(&mlx);
    if(load_image(&mlx))
    {
        write(2, "Failed to load image\n", 21);
		exit(1);
    }
	load_mlx(&mlx);
    mlx.start = 1;
    mlx_loop_hook(mlx.mlx, event_win, &mlx);
    mlx_loop_hook(mlx.mlx, drow_player, &mlx);
    mlx_loop(mlx.mlx);
    mlx_terminate(mlx.mlx);
}
