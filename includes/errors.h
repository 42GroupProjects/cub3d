/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 00:00:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/06/22 18:18:10 by lwittwer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
#define ERRORS_H

/* Error messages */
#define ERR_MALLOC "Memory allocation failed"
#define ERR_FILE_OPEN "Failed to open file"
#define ERR_FILE_READ "Failed to read file"
#define ERR_INVALID_ARG "Invalid arguments"
#define ERR_MAP_NOT_RECT "Map is not rectangular"
#define ERR_MAP_NOT_CLOSED "Map must be closed by walls"
#define ERR_MAP_INVALID "Map is invalid"
#define ERR_MAP_NO_PLAYER "Map must contain exactly one player (N/S/E/W)"
#define ERR_MAP_NO_EXIT "Map must contain exactly one exit (E)"
#define ERR_MAP_NO_COLLECT "Map must contain at least one collectible (C)"
#define ERR_MAP_NO_PATH "Map must have a valid path"
#define ERR_MAP_EMPTY_LINE "Map contains empty line(s)"
#define ERR_MAP_EMPTY "Map is empty"
#define ERR_MLX_INIT "MLX initialization failed"
#define ERR_MLX_WINDOW "MLX window creation failed"
#define ERR_MLX_IMAGE "Failed to load asset"
#define ERR_MLX_DATA "Failed to get data address"
#define ERR_INVALID_EXT "Invalid file extension (must be .cub)"

/* Header (textures + colors) errors */
#define ERR_DUP_TEX "Duplicate texture identifier (NO/SO/WE/EA)"
#define ERR_EMPTY_TEX "Empty texture path"
#define ERR_MISSING_TEX "Missing texture identifier (need NO, SO, WE, EA)"
#define ERR_DUP_COLOR "Duplicate color identifier (F/C)"
#define ERR_BAD_COLOR "Invalid color (expected R,G,B with each 0-255)"
#define ERR_MISSING_COLOR "Missing floor (F) or ceiling (C) color"
#define ERR_NO_MAP "Map content is missing"

#endif
