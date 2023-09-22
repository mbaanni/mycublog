/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singelton_object.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtaib <mtaib@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 16:35:14 by mtaib             #+#    #+#             */
/*   Updated: 2023/09/11 11:19:52 by mtaib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cub3D.h"

t_mapInfo*		getType(void)
{
	static t_mapInfo 	*mapInfo;

	if (!mapInfo)
	{
		mapInfo = malloc(sizeof(t_mapInfo));
		if (!mapInfo)
			return (0);
		mapInfo->directions = NULL;
		mapInfo->map = NULL;
		mapInfo->dirNbs = 0;
	}
	return (mapInfo);
}
