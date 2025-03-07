/*
** EPITECH PROJECT, 2024
** tiles_updates
** File description:
** Functions to update the map's isometric tiles.
*/

#include "../include/header.h"

static void tiles_rotation_exceptions_1(int *pos[2], int i,
    int size, cam_t *cam)
{
    int angle = (int)(cam->rotation + 360) % 360;
    int cases[4] = {i % (size - 1), i / (size - 1),
    (size - 1) - i % (size - 1), (size - 1) - i / (size - 1)};
    int *x = pos[0];
    int *y = pos[1];

    *x = cases[((angle % 180 / 90) - (angle % 180 / 45) % 2)
    * (angle % 180 / 90 * 2 - 1) + (angle / 180 * 2)];
    *y = cases[(((angle + 90) % 180 / 90) - ((angle + 90) % 180 / 45) % 2)
    * ((angle + 90) % 180 / 90 * 2 - 1) + ((angle + 90) % 360 / 180 * 2)];
}

static void tiles_rotation_exceptions_2(int *ix, int *iy,
    int val[3], cam_t *cam)
{
    int angle = (int)(cam->rotation + 360) % 360;
    int k = val[2];
    int cases[4] = {((k / 2) - k % 2) * (k / 2 * 2 - 1), k / 2,
    -((k / 2) - k % 2) * (k / 2 * 2 - 1), -k / 2};
    int x = val[0];
    int y = val[1];

    *ix = x + cases[(angle + 45) % 360 / 180 * 2]
    - ((angle % 180 - 45) / 90) * (angle / 180 * 2 - 1);
    *iy = y + cases[(angle + 45) % 360 / 180 * 2 + 1]
    + (((angle + 45) % 180 - 45) / 90) * (angle / 180 * 2 - 1);
}

void object_turning(sprite_t *obj, cam_t *cam)
{
    int angle = (int)(cam->rotation + 360) % 360;
    sfIntRect *rect = &obj->rect;
    sfVector2u size = sfTexture_getSize(obj->texture);
    int frx = size.x / rect->width;
    int fry = size.y / rect->height;

    rect->left = rect->width * ((int)((frx * fry)
    * ((float)angle / 360)) % frx);
    rect->top = rect->height * (int)(fry * ((float)angle / 360));
}

static void draw_vertex_object(sfVector2i pos, world_t *world,
    sfVertex vertex, cam_t *cam)
{
    sfVector2i curs = GAMESTUFF->curs_coords;
    sprite_t *obj = world->objects[pos.y][pos.x];

    if (obj == NULL)
        return;
    sfSprite_setOrigin(obj->sprite,
    (sfVector2f){obj->rect.width / 2.0, obj->rect.height});
    obj->pos = vertex.position;
    obj->scale = (sfVector2f){cam->zoom / 20.0, cam->zoom / 20.0};
    obj->angle = 0;
    obj->color = sfWhite;
    object_turning(obj, cam);
    if (GAMESTUFF->act == PLACE && GAMESTUFF->curs_state == MINIMAP
    && abs(curs.x - pos.x) <= GAMESTUFF->area
    && abs(curs.y - pos.y) <= GAMESTUFF->area) {
        obj->color.a = sin(TIME * 7) * 35 + 220;
        obj->scale.x *= sin(TIME * 3) / 10.0 + 1;
        obj->scale.y *= sin(TIME * 3 + 0.75) / 10.0 + 1;
    }
    draw_sprite(obj);
}

static void draw_arrow(int x, int y, sfVertex **vertex)
{
    sfVector2i curs = GAMESTUFF->curs_coords;

    if (GAMESTUFF->curs_state == MINIMAP
    && curs.x == x && curs.y == y) {
        draw_sprite(get_sprite("arrow"));
    }
}

void set_texture(int x, int y, world_t *world, int angle)
{
    x -= angle / 180;
    y -= ((angle / 180) - angle / 90 % 2) * (angle / 180 * 2 - 1);
    if (world->textures[y][x] == NULL)
        STATE->texture = NULL;
    else
        STATE->texture = world->textures[y][x]->texture;
}

sfColor get_defaultvertexcolor(float height)
{
    sfColor col = sfBlack;

    if (height > 20.0)
        col = (sfColor){(height - 20) + 210,
        (height - 20) + 205, (height - 20) + 200, 255};
    if (height > 1.0 && height <= 20.0)
        col = (sfColor){(height - 1) * 4,
        (height - 1) + 80, (height - 1) * 3, 255};
    if (height > -4.0 && height <= 1.0)
        col = (sfColor){height * 5 + 200,
        height * 7 + 175, height * 10 + 150, 255};
    if (height <= -4.0)
        col = (sfColor){height * 3 + 125,
        height * 3 + 100, height * 2 + 75, 255};
    return col;
}

void update_vertexcolor(int x, int y, world_t *world, sfVertex **vertex)
{
    sfVector2i curs = GAMESTUFF->curs_coords;
    float height = world->map[y][x];
    int area = GAMESTUFF->area;
    int col;

    if (STATE->texture == NULL) {
        vertex[y][x].color = get_defaultvertexcolor(height);
    } else {
        col = (1 - SQ(1 - (height + 40) / 100)) * 255;
        vertex[y][x].color = (sfColor){col, col, col, 255};
    }
    if (GAMESTUFF->curs_state == MINIMAP && x >= curs.x - area
    && x <= curs.x + area && y >= curs.y - area && y <= curs.y + area) {
        vertex[y][x].color.r = (vertex[y][x].color.r + 127) / 1.5;
        vertex[y][x].color.g = (vertex[y][x].color.g + 127) / 1.5;
        vertex[y][x].color.b = (vertex[y][x].color.b + 127) / 1.5;
    }
}

static sfVertex update_tiles_vertexorder(int val[3], world_t *world,
    sfVertex **vertex, cam_t *cam)
{
    int k = val[2];
    int ix;
    int iy;

    tiles_rotation_exceptions_2(&ix, &iy, val, cam);
    update_vertexcolor(ix, iy, world, vertex);
    vertex[iy][ix].texCoords = (sfVector2f){((k / 2) - k % 2)
    * (k / 2 * 2 - 1) * 50, k / 2 * 50};
    return vertex[iy][ix];
}

void update_tiles(world_t *world, sfVertex **vertex,
    sfVertexArray *tile, cam_t *cam)
{
    float **map = world->map;
    int size = world->size;
    int x;
    int y;

    for (int i = 0; i < SQ(size - 1); i++) {
        sfVertexArray_clear(tile);
        tiles_rotation_exceptions_1((int *[2]){&x, &y}, i, size, cam);
        draw_vertex_object((sfVector2i){x, y}, world, vertex[y][x], cam);
        draw_arrow(x, y, vertex);
        set_texture(x, y, world, (int)(cam->rotation + 360) % 360);
        for (int k = 0; k < 4; k++) {
            sfVertexArray_append(tile,
            update_tiles_vertexorder((int[3]){x, y, k}, world, vertex, cam));
        }
        sfRenderWindow_drawVertexArray(WINDOW, tile, STATE);
    }
}
