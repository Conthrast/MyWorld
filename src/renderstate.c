/*
** EPITECH PROJECT, 2024
** renderstate
** File description:
** RenderState related functions.
*/

#include "../include/header.h"

sfRenderStates **get_renderstate(void)
{
    static sfRenderStates *state = NULL;

    return &state;
}

void init_renderstate(void)
{
    sfRenderStates *state = malloc(sizeof(sfRenderStates));

    if (state == NULL)
        return;
    state->shader = NULL;
    state->blendMode = sfBlendAlpha;
    state->transform = sfTransform_Identity;
    state->texture = NULL;
    *get_renderstate() = state;
}
