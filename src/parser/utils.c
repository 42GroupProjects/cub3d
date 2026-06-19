#include "cub3d.h"

/* Print "Error\n<msg>" to stderr and return 0 so callers can do:
 *   return (parse_error(ERR_...));                                          */
int	parse_error(char *msg)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
	return (0);
}

/* Advance past leading spaces/tabs and return the new position. */
char	*skip_spaces(char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}

/* Return 1 if line begins with identifier `id` followed by a space/tab.
 * Guards against "NORTH" being mistaken for the "NO" identifier.          */
int	match_id(char *line, char *id)
{
	int	len;

	len = ft_strlen(id);
	if (ft_strncmp(line, id, len) != 0)
		return (0);
	if (line[len] != ' ' && line[len] != '\t')
		return (0);
	return (1);
}

/* Return 1 if line is empty or only whitespace. */
int	is_blank(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}
