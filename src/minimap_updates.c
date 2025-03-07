/*
** EPITECH PROJECT, 2024
** minimap_updates
** File description:
** Functions to update the minimap.
*/

#include "../include/header.h"

static sfVertex draw_minimap_vertexorder(int val[3], world_t *world,
    sfVertex **vertex, int grid)
{
    sfVector2f pos;
    int x = val[0];
    int y = val[1];
    int k = val[2];
    int ix = x + ((k / 2) - k % 2) * (k / 2 * 2 - 1);
    int iy = y + k / 2;

    pos.x = ix * 250 / (world->size - 1);
    pos.y = iy * 250 / (world->size - 1) + 30;
    vertex[iy][ix].position = pos;
    if (grid)
        vertex[iy][ix].color = (sfColor){255, 255, 255, 25};
    else
        update_vertexcolor(ix, iy, world, vertex);
    vertex[iy][ix].texCoords = (sfVector2f){((k / 2) - k % 2)
    * (k / 2 * 2 - 1) * 50, k / 2 * 50};
    return vertex[iy][ix];
}

static void draw_minimap(world_t *world, sfVertex **vertex,
    sfVertexArray *tile)
{
    int x;
    int y;

    for (int i = 0; i < SQ(world->size - 1); i++) {
        sfVertexArray_clear(tile);
        x = i % (world->size - 1);
        y = i / (world->size - 1);
        set_texture(x, y, world, 0);
        for (int k = 0; k < 4; k++) {
            sfVertexArray_append(tile,
            draw_minimap_vertexorder((int[3]){x, y, k}, world, vertex, 0));
        }
        sfRenderWindow_drawVertexArray(WINDOW, tile, STATE);
    }
}

static void draw_minimap_vertex_object(sfVector2i pos, world_t *world,
    sfVertex vertex)
{
    sfVector2i curs = GAMESTUFF->curs_coords;
    sprite_t *obj = world->objects[pos.y][pos.x];

    if (obj == NULL)
        return;
    sfSprite_setOrigin(obj->sprite,
    (sfVector2f){obj->rect.width / 2.0, obj->rect.height / 2.0});
    obj->pos = vertex.position;
    obj->scale = (sfVector2f){5.0 / (float)world->size,
    5.0 / (float)world->size};
    obj->angle = 0;
    obj->color = sfWhite;
    draw_sprite(obj);
}

static void draw_minimap_grid(world_t *world, sfVertex **vertex,
    sfVertexArray *tile)
{
    int x;
    int y;

    sfVertexArray_setPrimitiveType(tile, sfLineStrip);
    for (int i = 0; i < SQ(world->size - 1); i++) {
        sfVertexArray_clear(tile);
        x = i % (world->size - 1);
        y = i / (world->size - 1);
        for (int k = 0; k < 4; k++) {
            sfVertexArray_append(tile,
            draw_minimap_vertexorder((int[3]){x, y, k}, world, vertex, 1));
        }
        draw_minimap_vertex_object((sfVector2i){x, y}, world, vertex[y][x]);
        sfRenderWindow_drawVertexArray(WINDOW, tile, NULL);
    }
}

void update_minimap(world_t *world, sfVertex **vertex, sfVertexArray *tile)
{
    if (GAMESTUFF->fullscreen)
        return;
    draw_minimap(world, vertex, tile);
    draw_minimap_grid(world, vertex, tile);
}
