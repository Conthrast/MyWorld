/*
** EPITECH PROJECT, 2024
** window_csfml
** File description:
** Window related functions.
*/

#include "../include/header_csfml.h"

sfRenderWindow **get_window(void)
{
    static sfRenderWindow *window;

    return &window;
}

void create_window(unsigned int width, unsigned int height, char const *name)
{
    sfVideoMode video_mode;

    video_mode.width = width;
    video_mode.height = height;
    video_mode.bitsPerPixel = 32;
    *get_window() = sfRenderWindow_create(video_mode, name,
    sfDefaultStyle, NULL);
    sfRenderWindow_setFramerateLimit(*get_window(), 60);
}

void destroy_window(void)
{
    sfRenderWindow_destroy(*get_window());
}
