/*
** EPITECH PROJECT, 2024
** world_loadnsave
** File description:
** World (loading and saving) related functions.
*/

#include "../include/header.h"

static void load_world_coords(world_t *world, FILE *file)
{
    int size = world->size;
    float **map = malloc(sizeof(float *) * (size + 1));
    char *data;
    char *line = NULL;
    size_t s = 0;

    getline(&line, &s, file);
    for (int i = 0; i < size; i++) {
        getline(&line, &s, file);
        data = strtok(line, " \n");
        map[i] = malloc(sizeof(float) * (size + 1));
        for (int j = 0; j < size; j++) {
            map[i][j] = atof(data);
            data = strtok(NULL, " \n");
        }
        map[i][size] = -1;
    }
    map[size] = NULL;
    world->map = map;
    free(line);
}

static sprite_t ***load_world_spritemap(int size, FILE *file)
{
    sprite_t ***map = malloc(sizeof(sprite_t **) * size);
    char *data;
    char *line = NULL;
    size_t s = 0;

    if (map == NULL)
        return NULL;
    for (int i = 0; i < size; i++) {
        getline(&line, &s, file);
        data = strtok(line, " \n");
        map[i] = malloc(sizeof(sprite_t *) * size);
        if (map[i] == NULL)
            return NULL;
        for (int j = 0; j < size; j++) {
            map[i][j] = get_sprite(data);
            data = strtok(NULL, " \n");
        }
    }
    free(line);
    return map;
}

world_t *load_world(char *name)
{
    FILE *file;
    world_t *world = malloc(sizeof(world_t));
    char *path;

    if (world == NULL)
        return NULL;
    world->name = strdup(name);
    path = malloc(sizeof(char) * (strlen("worlds/")
    + strlen(name) + strlen(".legend") + 1));
    strcpy(path, "worlds/");
    strcat(path, world->name);
    strcat(path, ".legend");
    file = fopen(path, "r");
    fscanf(file, "size %d", &world->size);
    load_world_coords(world, file);
    world->objects = load_world_spritemap(world->size, file);
    world->textures = load_world_spritemap(world->size - 1, file);
    free(path);
    fclose(file);
    return world;
}

static void save_world_coords(world_t *world, FILE *file)
{
    float **map = world->map;
    int size = world->size;
    int x;
    int y;

    for (int i = 0; i < SQ(size); i++) {
        x = i % size;
        y = i / size;
        if (x == 0)
            fprintf(file, "\n");
        else
            fprintf(file, " ");
        fprintf(file, "%05.1f", map[y][x]);
    }
}

static void save_world_spritemap(int size, sprite_t ***map, FILE *file)
{
    int x;
    int y;

    for (int i = 0; i < SQ(size); i++) {
        x = i % size;
        y = i / size;
        if (x == 0)
            fprintf(file, "\n");
        else
            fprintf(file, " ");
        if (map[y][x] == NULL)
            fprintf(file, "x");
        else
            fprintf(file, "%s", map[y][x]->name);
    }
}

void save_world(world_t *world)
{
    FILE *file;
    char *path = malloc(sizeof(char) * (strlen("worlds/")
    + strlen(world->name) + strlen(".legend") + 1));

    if (path == NULL)
        return;
    strcpy(path, "worlds/");
    strcat(path, world->name);
    strcat(path, ".legend");
    file = fopen(path, "w+");
    if (file == NULL) {
        free(path);
        return;
    }
    fprintf(file, "size %d", world->size);
    save_world_coords(world, file);
    save_world_spritemap(world->size, world->objects, file);
    save_world_spritemap(world->size - 1, world->textures, file);
    free(path);
    fclose(file);
}

int valid_world(char *name)
{
    FILE *file;
    char *path = malloc(sizeof(char) * (strlen("worlds/")
    + strlen(name) + strlen(".legend") + 1));

    if (path == NULL)
        return 0;
    strcpy(path, "worlds/");
    strcat(path, name);
    strcat(path, ".legend");
    file = fopen(path, "r");
    free(path);
    if (file == NULL)
        return 0;
    fclose(file);
    return 1;
}
