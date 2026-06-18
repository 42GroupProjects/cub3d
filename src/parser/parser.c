#include "cub3d.h"

static int count_lines(char *filename)
{
    int fd;
    int count;
    char *line;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (-1);
    count = 0;
    line = get_next_line(fd);
    while (line)
    {
        count++;
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return (count);
}

static int check_extension(char *filename)
{
    int len;

    len = ft_strlen(filename);
    if (len < 4)
        return (0);
    return (ft_strncmp(filename + len - 4, ".cub", 4) == 0);
}

char **parse_map(char *filename)
{
    int fd;
    int i;
    int lines;
    char **map;

    if (!check_extension(filename))
        return (NULL);

    lines = count_lines(filename);
    if (lines <= 0)
        return (NULL);

    map = malloc(sizeof(char *) * (lines + 1));
    if (!map)
        return (NULL);

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (NULL);

    i = 0;
    while (i < lines)
    {
        map[i] = get_next_line(fd);
        i++;
    }
    map[i] = NULL;
    close(fd);
    return (map);
}