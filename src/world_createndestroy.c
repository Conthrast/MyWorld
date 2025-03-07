/*
** EPITECH PROJECT, 2024
** world_createndestroy
** File description:
** World (creating and destroying) related functions.
*/

#include "../include/header.h"

static float **float_map_gen(int size)
{
    float **map = malloc(sizeof(float *) * (size + 1));

    if (map == NULL)
        return NULL;
    for (int i = 0; i < size; i++) {
        map[i] = malloc(sizeof(float) * (size + 1));
        if (map[i] == NULL)
            return NULL;
        for (int j = 0; j < size; j++)
            map[i][j] = diceroll(0, 10) / 10.0;
        map[i][size] = -1;
    }
    map[size] = NULL;
    return map;
}

static sprite_t ***sprite_map_gen(int size)
{
    sprite_t ***map = malloc(sizeof(sprite_t **) * size);

    if (map == NULL)
        return NULL;
    for (int i = 0; i < size; i++) {
        map[i] = malloc(sizeof(sprite_t *) * size);
        if (map[i] == NULL)
            return NULL;
        for (int j = 0; j < size; j++)
            map[i][j] = NULL;
    }
    return map;
}

world_t *init_world(int size)
{
    world_t *world = malloc(sizeof(world_t));

    if (world == NULL)
        return NULL;
    world->name = strdup("new world");
    world->size = size;
    world->map = float_map_gen(size);
    world->objects = sprite_map_gen(size);
    world->textures = sprite_map_gen(size - 1);
    return world;
}

void destroy_world(world_t *world, sfVertex **vertex)
{
    for (int i = 0; i < world->size; i++)
        free(vertex[i]);
    free(vertex);
    for (int j = 0; world->map[j] != NULL; j++) {
        free(world->map[j]);
        free(world->objects[j]);
        if (j > 0)
            free(world->textures[j - 1]);
    }
    free(world->map);
    free(world->objects);
    free(world->textures);
    free(world->name);
    free(world);
}

void clear_world(world_t *world, sfVertex **vertex, cam_t *cam)
{
    char *camtweens[5] = {"camrot", "camtilt",
    "camzoom", "camcentx", "camcenty"};

    for (int i = 0; i < 5; i++) {
        if (get_tween(camtweens[i]) != NULL)
            destroy_tween(camtweens[i]);
    }
    destroy_world(world, vertex);
    free(cam);
    GAMESTUFF->curs_coords = (sfVector2i){0, 0};
    GAMESTUFF->curs_state = NOSTATE;
}
