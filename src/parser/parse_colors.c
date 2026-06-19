#include "cub3d.h"

/* Count entries in a NULL-terminated array. */
static int	count_words(char **words)
{
	int	n;

	n = 0;
	while (words[n])
		n++;
	return (n);
}

/* Valid byte = non-empty, all digits, value in [0, 255]. */
static int	is_valid_byte(char *s)
{
	int	i;

	if (!s[0])
		return (0);
	i = 0;
	while (s[i])
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (0);
		i++;
	}
	if (ft_atoi(s) < 0 || ft_atoi(s) > 255)
		return (0);
	return (1);
}

/* Validate exactly 3 byte components and store them into rgb[0..2]. */
static int	fill_rgb(int *rgb, char **parts)
{
	int	i;

	if (count_words(parts) != 3)
		return (0);
	i = 0;
	while (i < 3)
	{
		if (!is_valid_byte(parts[i]))
			return (0);
		rgb[i] = ft_atoi(parts[i]);
		i++;
	}
	return (1);
}

/* Parse "R,G,B" into rgb. *flag guards against a duplicate F/C line. */
int	set_color(int *rgb, char *value, int *flag)
{
	char	**parts;

	if (*flag)
		return (parse_error(ERR_DUP_COLOR));
	value = skip_spaces(value);
	parts = ft_split(value, ',');
	if (!parts)
		return (parse_error(ERR_MALLOC));
	if (!fill_rgb(rgb, parts))
		return (free_map(parts), parse_error(ERR_BAD_COLOR));
	free_map(parts);
	*flag = 1;
	return (1);
}
