#include "cub3d.h"

char **append_line(char **map, char *line)
{
    char **new_map;
    int i;
    int size;

    size = 0;
    while (map && map[size])
        size++;

    new_map = malloc(sizeof(char *) * (size + 2));
    if (!new_map)
        return (NULL);

    i = 0;
    while (i < size)
    {
        new_map[i] = map[i];
        i++;
    }

    new_map[i] = ft_strdup(line);
    new_map[i + 1] = NULL;

    free(map);
    return (new_map);
}

char **load_map(char *file)
{
    int     fd;
    char    *line;
    char    **map;
    char    **tmp;

    map = NULL;
    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (NULL);
    while ((line = get_next_line(fd)))
    {
        tmp = append_line(map, line);
        free(line);
        if (!tmp)
        {
            free_map(map);
            close(fd);
            return (NULL);
        }
        map = tmp;
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
