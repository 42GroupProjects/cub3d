#include "cub3d.h"

int check_file(char *file)
{
    int fd;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (0);
    close(fd);
    return (1);
}

int check_not_directory(char *file)
{
    int fd;
    char buffer;
    int ret;

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (0);

    ret = read(fd, &buffer, 1);
    close(fd);

    if (ret < 0)
        return (0);

    return (1);
}

int check_extension(char *file)
{
    int len;

    if (!file)
        return (0);

    len = ft_strlen(file);
    if (len < 4)
        return (0);

    if (ft_strcmp(file + len - 4, ".cub") != 0)
        return (0);

    return (1);
}

int validate_file(char *file)
{
    int fd;

    if (!file)
        return (0);

    if (!check_extension(file))
        return (0);

    if (!check_file(file))
        return (0);

    if (!check_not_directory(file))
        return (0);

    fd = open(file, O_RDONLY);
    if (fd < 0)
        return (0);

    close(fd);
    return (1);
}
