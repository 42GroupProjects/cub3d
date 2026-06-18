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

char **load_map(char *filename)
{
    int fd;
    int lines;
    char **map;
    int i;

    lines = count_lines(filename);
    if (lines <= 0)
        return (NULL);

    map = malloc(sizeof(char *) * (lines + 1));
    if (!map)
        return (NULL);

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        free(map);
        return (NULL);
    }

    i = 0;
    while (i < lines)
    {
        map[i] = get_next_line(fd);
        if (!map[i])
        {
            free_map(map);
            close(fd);
            return (NULL);
        }
        i++;
    }
    map[i] = NULL;
    close(fd);
    return (map);
}

char **parse_map(char *filename)
{
    char **map;

    if (!validate_file(filename))
        return (NULL);

    map = load_map(filename);
    if (!map)
        return (NULL);

    if (!validate_characters(map))
    {
        free_map(map);
        return (NULL);
    }

    if (validate_player_count(map) != 1)
    {
        free_map(map);
        return (NULL);
    }

    return (map);
}
