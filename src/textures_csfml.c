/*
** EPITECH PROJECT, 2024
** textures_csfml
** File description:
** Texture related functions.
*/

#include "../include/header_csfml.h"

texture_t **get_texturelist(void)
{
    static texture_t *texturelist = NULL;

    return &texturelist;
}

texture_t *get_texture(char const *name)
{
    texture_t *texture = *get_texturelist();

    while (texture != NULL) {
        if (strcmp(texture->name, name) == 0)
            return texture;
        texture = texture->next;
    }
    return NULL;
}

texture_t *make_texture(char *name, char const *file)
{
    texture_t *texture = malloc(sizeof(texture_t));

    if (texture == NULL)
        return NULL;
    texture->name = strdup(name);
    texture->texture = sfTexture_createFromFile(file, NULL);
    texture->size.x = sfTexture_getSize(texture->texture).x;
    texture->size.y = sfTexture_getSize(texture->texture).y;
    texture->next = *get_texturelist();
    *get_texturelist() = texture;
    return texture;
}

static void free_texture(texture_t *texture)
{
    sfTexture_destroy(texture->texture);
    free(texture->name);
    free(texture);
}

static void destroy_firsttexture(void)
{
    texture_t *next = NULL;

    if (*get_texturelist() == NULL)
        return;
    next = (*get_texturelist())->next;
    free_texture(*get_texturelist());
    *get_texturelist() = next;
}

static void destroy_lasttexture(void)
{
    texture_t *list = *get_texturelist();

    if (list->next == NULL) {
        free_texture(*get_texturelist());
        return;
    }
    while (list->next->next != NULL)
        list = list->next;
    free_texture(list->next);
    list->next = NULL;
}

void destroy_texture(texture_t *texture)
{
    texture_t *list = *get_texturelist();
    texture_t *tmp = NULL;

    if (strcmp(list->name, texture->name) == 0) {
        destroy_firsttexture();
        return;
    }
    while (list->next != NULL) {
        if (strcmp(list->next->name, texture->name) == 0)
            break;
        list = list->next;
    }
    if (list->next == NULL) {
        destroy_lasttexture();
        return;
    }
    tmp = list->next;
    list->next = tmp->next;
    free_texture(tmp);
}
