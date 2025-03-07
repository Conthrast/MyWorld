/*
** EPITECH PROJECT, 2024
** spritesother_csfml
** File description:
** More sprite related functions.
*/

#include "../include/header_csfml.h"

void draw_allsprites(type_t type)
{
    sprite_t *sprite = *get_spritelist();

    while (sprite != NULL) {
        if (sprite->type == type)
            draw_sprite(sprite);
        sprite = sprite->next;
    }
}
