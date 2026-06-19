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

#endif
