/*
** EPITECH PROJECT, 2024
** hud_updates
** File description:
** Functions to update the UI elements.
*/

#include "../include/header.h"

static void selected(sprite_t *sprite)
{
    sprite->scale =
    (sfVector2f){50.0 / sprite->rect.width, 50.0 / sprite->rect.height};
    if (sprite == GAMESTUFF->item) {
        sprite->angle = cos(TIME * 2.0) * 5.0;
        sprite->color = sfWhite;
    } else {
        sprite->scale.x *= 0.9;
        sprite->scale.y *= 0.9;
        sprite->angle = 0;
        sprite->color = (sfColor){200, 200, 200, 150};
    }
}

void raise_ui(void)
{
}

void paint_ui(void)
{
    sprite_t *sprite = *get_spritelist();
    int i = 0;

    while (sprite != NULL) {
        if (sprite->type == TEXTURE) {
            sfSprite_setOrigin(sprite->sprite,
            (sfVector2f){sprite->rect.width / 2.0, sprite->rect.height / 2.0});
            sprite->pos = (sfVector2f){291 + (i % 9) * 59, 480 + (i / 9) * 60};
            selected(sprite);
            draw_sprite(sprite);
            i++;
        }
        sprite = sprite->next;
    }
}

void place_ui(void)
{
    sprite_t *sprite = *get_spritelist();
    int i = 0;

    while (sprite != NULL) {
        if (sprite->type == OBJECT) {
            sfSprite_setOrigin(sprite->sprite,
            (sfVector2f){sprite->rect.width / 2.0, sprite->rect.height / 2.0});
            sprite->pos = (sfVector2f){291 + (i % 9) * 59, 480 + (i / 9) * 60};
            selected(sprite);
            draw_sprite(sprite);
            i++;
        }
        sprite = sprite->next;
    }
}
