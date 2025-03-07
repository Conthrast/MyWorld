/*
** EPITECH PROJECT, 2024
** gamestuff
** File description:
** Gamestate related functions.
*/

#include "../include/header.h"

game_t **get_gamestuff(void)
{
    static game_t *game = NULL;

    return &game;
}

game_t *init_gamestuff(void)
{
    game_t *game = malloc(sizeof(game_t));

    if (game == NULL)
        return NULL;
    game->act = RAISE;
    game->area = 3;
    game->item = NULL;
    game->curs_coords = (sfVector2i){0, 0};
    game->curs_state = NOSTATE;
    game->fullscreen = 0;
    *get_gamestuff() = game;
    return game;
}
