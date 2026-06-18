/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 00:00:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2025/10/27 20:17:08 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

/* Error messages */
# define ERR_MALLOC				"Memory allocation failed"
# define ERR_FILE_OPEN			"Failed to open file"
# define ERR_FILE_READ			"Failed to read file"
# define ERR_INVALID_ARG		"Invalid arguments"
# define ERR_MAP_NOT_RECT		"Map is not rectangular"
# define ERR_MAP_NOT_CLOSED		"Map must be closed by walls"
# define ERR_MAP_INVALID        "Map is invalid"
# define ERR_MAP_NO_PLAYER		"Map must contain exactly one player (P)"
# define ERR_MAP_NO_EXIT		"Map must contain exactly one exit (E)"
# define ERR_MAP_NO_COLLECT		"Map must contain at least one collectible (C)"
# define ERR_MAP_NO_PATH		"Map must have a valid path"
# define ERR_MAP_EMPTY_LINE		"Map contains empty line(s)"
# define ERR_MAP_EMPTY			"Map is empty"
# define ERR_MLX_INIT			"MLX initialization failed"
# define ERR_MLX_WINDOW			"MLX window creation failed"
# define ERR_MLX_IMAGE			"Failed to load asset"
# define ERR_INVALID_EXT		"Invalid file extension (must be .ber)"

#endif
