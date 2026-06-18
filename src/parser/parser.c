#include "cub3d.h"

char **load_map(char *file)
{
    int fd;
    char *line;
    char **map;

    map = NULL;
    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (NULL);

    line = get_next_line(fd);
    while (line)
    {
        map = append_line(map, line);
        free(line);
        line = get_next_line(fd);
    }

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
