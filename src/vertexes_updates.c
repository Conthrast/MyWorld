/*
** EPITECH PROJECT, 2024
** vertexes_updates
** File description:
** Functions to update the map's isometric vertexes.
*/

#include "../include/header.h"

static sfVector2f vertex_apply_offsets(float coords[3], float rad,
    int size, cam_t *cam)
{
    sfVector2f pos;

    pos.x = (coords[0] + cam->offset.x) * cam->zoom + 400;
    pos.y = (coords[1] + cam->offset.y) * cam->zoom + 350 - SQ(cam->tilt) * 50;
    pos.x += (1 - GAMESTUFF->fullscreen) * 125;
    pos.y -= (1 - GAMESTUFF->fullscreen) * 65;
    pos.y += cam->zoom / 1.2;
    pos.x -= cos(rad + 0.785) * (cam->zoom / 1.5) * size;
    pos.y -= sin(rad + 0.785) * (cam->zoom / 1.5) * cam->tilt * size;
    pos.y -= coords[2] * (INTENSITY * (1 - SQ(cam->tilt))) * cam->zoom / 50;
    return pos;
}

void update_vertexes(world_t *world, sfVertex **vertex, cam_t *cam)
{
    float rad = cam->rotation * M_PI / 180.0;
    float **map = world->map;
    int size = world->size;
    sfVector2f pos;
    sfColor col;
    int x;
    int y;

    for (int i = 0; i < SQ(size); i++) {
        x = i % (size);
        y = i / (size);
        pos.x = ((x * cos(rad)) - (y * sin(rad)));
        pos.y = ((x * sin(rad)) * cam->tilt + (y * cos(rad)) * cam->tilt);
        pos = vertex_apply_offsets(
        (float[3]){pos.x, pos.y, map[y][x]}, rad, size, cam);
        vertex[y][x].position = pos;
    }
}

void vertex_indicate(world_t *world,
    sfVertex **vertex, cam_t *cam)
{
    sfVector2i curs = GAMESTUFF->curs_coords;
    int area = GAMESTUFF->area;
    sprite_t *arrow = get_sprite("arrow");
    int x = curs.x;
    int y = curs.y;
    sprite_t *obj = world->objects[y][x];

    arrow->draw = 1;
    arrow->pos = vertex[y][x].position;
    arrow->pos.y -= sin(TIME * 3) * 5;
    if (obj != NULL)
        arrow->pos.y -= obj->rect.height * cam->zoom / 20.0;
    arrow->scale.x = SQ((cam->zoom + 10) / 75.0) + 2;
    arrow->scale.y = SQ((cam->zoom + 10) / 75.0) + 2;
    arrow->color.a = sin(TIME * 4) * 60 + 180;
    animate(arrow, 0, 3, 2);
}
