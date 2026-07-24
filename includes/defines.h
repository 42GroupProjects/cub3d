/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thanh-ng <thanh-ng@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 20:55:00 by thanh-ng          #+#    #+#             */
/*   Updated: 2026/07/19 20:55:00 by thanh-ng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

/*
** Function outcomes. Operations (things that can fail several ways) return
** one of these; simple predicates ("is this valid?") return TRUE / FALSE.
** OOM is kept distinct from FAILURE so a malloc failure can be told apart
** from a bad-input failure, mirroring minishell's SUCCESS/FAILURE/OOM.
*/
# define SUCCESS	0	/* operation completed without error          */
# define FAILURE	1	/* recoverable error: invalid file / map      */
# define OOM		-2	/* out of memory: malloc/calloc returned NULL */

/* Logical booleans, distinct from SUCCESS/FAILURE so predicates read well. */
# define FALSE		0
# define TRUE		1

/* Parser limits. */
# define RGB_COUNT			3	/* components in an "R,G,B" color   */
# define RGB_MAX			255	/* max value of one color component */
# define FILE_EXT_LENGTH	4	/* length of ".cub"                 */
# define TEX_ID_LEN			2	/* length of NO/SO/WE/EA identifier */

/* Color & graphics (consumed later by the raycaster). */
# define ONE_OVER_255 0.00392156862f
# define BYTES_PER_PIXEL 4
# define BITS_IN_BYTE 8
# define PATH_BUFFER_SIZE 256
# define STRING_BUFFER_SIZE 32
# define GREEN "\033[0;32m"
# define CYAN "\033[1;96m"
# define RESET "\033[0m"

/* Window size and tile sizes */
# define WIDTH 600
# define HEIGHT 600
# define TS 32

/* Navigation input (Linux X11 keycodes) */
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_L 108
# define KEY_SPACE 32
# define ARROW_LEFT 65361
# define ARROW_RIGHT 65363
# define MOUSE_LEFT 1

/* Raycast defines */
# define STEP 0.1

/* Movement speeds (units / radians per second; scaled by dt) */
# define MOVE_SPEED 3.0
# define ROT_SPEED 1.8
# define COLLISION_PAD 0.2
# define DT_DEFAULT 0.0166667
# define DT_MAX 0.05

/* Bonus: mouse look (radians per pixel of horizontal drift) */
# define MOUSE_SENS 0.003

/* Bonus: distance fog on walls */
# define FOG_DENSITY 0.18
# define FOG_MIN 0.18
# define FOG_SIDE 0.7

/* Bonus: crosshair */
# define CROSS_SIZE 8
# define CROSS_GAP 2
# define CROSS_COLOR 0xFFFFFF

/* Bonus: floor/ceiling horizon gradient */
# define BG_GRAD 0.55
# define BG_GRAD_MIN 0.35

/* Bonus: weapon overlay (magenta key) */
# define WEAPON_PATH "textures/weapon.xpm"
# define WEAPON_KEY 0x00FF00FF
# define WEAPON_SCALE 2
# define WEAPON_BOB_SPEED 10.8
# define WEAPON_BOB_DECAY 6.0
# define WEAPON_BOB_X 10.0
# define WEAPON_BOB_Y 6.0
# define FIRE_KICK_TIME 0.12
# define FIRE_KICK_PX 16

/* Bonus: head bob (pixels of vertical view shift) */
# define HEAD_BOB_SPEED 10.0
# define HEAD_BOB_AMT 4

/* Bonus: minimap */
# define KEY_M 109
# define KEY_M_UP 77
# define MM_SIZE 140
# define MM_MARGIN 12
# define MM_ZOOM 0.08
# define MM_WALL 0x3A3A3A
# define MM_FLOOR 0x1A1A1A
# define MM_VOID 0x0A0A0A
# define MM_PLAYER 0xE74C3C
# define MM_DIR 0xF1C40F
# define MM_BORDER 0x95A5A6
# define MM_FULL_ALPHA 150
# define MM_TILE_ALPHA 200
#endif
