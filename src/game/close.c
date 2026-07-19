/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwittwer <lwittwer@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 17:56:18 by lwittwer          #+#    #+#             */
/*   Updated: 2026/07/14 18:05:43 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	on_x(t_cub *c)
{
	on_close(c);
	return (0);
}

void	on_close(t_cub *c)
{
	// FIXME: also free_config(c->config) before exit, or return to main — this path leaks textures/map
	free_cub_struct(c);
	exit(1);
}
