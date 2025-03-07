/*
** EPITECH PROJECT, 2024
** init_other
** File description:
** Functions to initialize the program's other cool stuff.
*/

#include "../include/header.h"

cam_t *init_cam(int size)
{
    cam_t *cam = malloc(sizeof(cam_t));

    if (cam == NULL)
        return NULL;
    cam->offset = (sfVector2f){0, 0};
    cam->rotation = 360;
    cam->tilt = 0.5;
    cam->zoom = 0.0;
    cam->zoomdef = 400 / size;
    if (GAMESTUFF->fullscreen)
        cam->zoomdef *= 1.4;
    cam_reset(cam);
    return cam;
}

sfVertex **init_vertexes(world_t *world)
{
    float **map = world->map;
    int size = world->size;
    sfVertex **vertex = malloc(sizeof(sfVertex *) * size);
    int x;
    int y;

    if (vertex == NULL)
        return NULL;
    for (int i = 0; i < SQ(size); i++) {
        x = i % size;
        y = i / size;
        if (x == 0)
            vertex[y] = malloc(sizeof(sfVertex) * size);
    }
    return vertex;
}
