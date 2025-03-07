/*
** EPITECH PROJECT, 2024
** sprites_csfml
** File description:
** Sprite related functions.
*/

#include "../include/header_csfml.h"

sprite_t **get_spritelist(void)
{
    static sprite_t *spritelist = NULL;

    return &spritelist;
}

sprite_t *get_sprite(char const *name)
{
    sprite_t *sprite = *get_spritelist();

    while (sprite != NULL) {
        if (strcmp(sprite->name, name) == 0)
            return sprite;
        sprite = sprite->next;
    }
    return NULL;
}

sprite_t *make_sprite(char *name, char const *file, int x, int y)
{
    sprite_t *sprite = malloc(sizeof(sprite_t));

    if (sprite == NULL)
        return NULL;
    sprite->name = strdup(name);
    sprite->sprite = sfSprite_create();
    sprite->texture = sfTexture_createFromFile(file, NULL);
    sfSprite_setTexture(sprite->sprite, sprite->texture, sfTrue);
    sprite->pos = (sfVector2f){x, y};
    sprite->scale = (sfVector2f){1, 1};
    sprite->angle = 0;
    sprite->color = sfWhite;
    sprite->rect = (sfIntRect){0, 0, sfTexture_getSize(sprite->texture).x,
    sfTexture_getSize(sprite->texture).y};
    sprite->type = HUD;
    sprite->draw = 1;
    sprite->next = *get_spritelist();
    *get_spritelist() = sprite;
    return sprite;
}

void draw_sprite(sprite_t *sprite)
{
    if (sprite->draw) {
        sfSprite_setPosition(sprite->sprite, sprite->pos);
        sfSprite_setTextureRect(sprite->sprite, sprite->rect);
        sfSprite_setScale(sprite->sprite, sprite->scale);
        sfSprite_setRotation(sprite->sprite, sprite->angle);
        sfSprite_setColor(sprite->sprite, sprite->color);
        sfRenderWindow_drawSprite(WINDOW, sprite->sprite, NULL);
    }
}

static void free_sprite(sprite_t *sprite)
{
    sfSprite_destroy(sprite->sprite);
    sfTexture_destroy(sprite->texture);
    free(sprite->name);
    free(sprite);
}

static void destroy_firstsprite(void)
{
    sprite_t *next = NULL;

    if (*get_spritelist() == NULL)
        return;
    next = (*get_spritelist())->next;
    free_sprite(*get_spritelist());
    *get_spritelist() = next;
}

static void destroy_lastsprite(void)
{
    sprite_t *list = *get_spritelist();

    if (list->next == NULL) {
        free_sprite(*get_spritelist());
        return;
    }
    while (list->next->next != NULL)
        list = list->next;
    free_sprite(list->next);
    list->next = NULL;
}

void destroy_sprite(sprite_t *sprite)
{
    sprite_t *list = *get_spritelist();
    sprite_t *tmp = NULL;

    if (strcmp(list->name, sprite->name) == 0) {
        destroy_firstsprite();
        return;
    }
    while (list->next != NULL) {
        if (strcmp(list->next->name, sprite->name) == 0)
            break;
        list = list->next;
    }
    if (list->next == NULL) {
        destroy_lastsprite();
        return;
    }
    tmp = list->next;
    list->next = tmp->next;
    free_sprite(tmp);
}
