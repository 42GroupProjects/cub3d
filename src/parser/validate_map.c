#include "cub3d.h"

int validate_characters(char **map)
{
    int i;
    int j;

    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (map[i][j] != '0' && map[i][j] != '1' && map[i][j] != '2' &&
                map[i][j] != 'N' && map[i][j] != 'S' && map[i][j] != 'E' &&
                map[i][j] != 'W' && map[i][j] != ' ')
                return (0);
            j++;
        }
        i++;
    }
    return (1);
}

int validate_player_count(char **map)
{
    int i;
    int j;
    int player_count;

    player_count = 0;
    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (map[i][j] == 'N' || map[i][j] == 'S' ||
                map[i][j] == 'E' || map[i][j] == 'W')
                player_count++;
            j++;
        }
        i++;
    }
    return (player_count == 1);
}
