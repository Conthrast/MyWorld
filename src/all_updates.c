/*
** EPITECH PROJECT, 2024
** all_updates
** File description:
** Functions to update all of the different elements of the program.
*/

#include "../include/header.h"

static void update_bg(void)
{
    sprite_t *bg = get_sprite("bg");
    sfVector2f pos = sfSprite_getOrigin(bg->sprite);

    bg->scale.x = (2 + cos(TIME / 10.0) * 0.7)
    * (0.7 + GAMESTUFF->fullscreen * 0.3);
    bg->scale.y = (2 + sin(TIME / 10.0) * 0.7)
    * (0.7 + GAMESTUFF->fullscreen * 0.3);
    pos.x = (1 + cos(TIME / 2.5)) * 100;
    pos.y = TIME * 75;
    pos.x = (int)pos.x % 100 + 450;
    pos.y = (int)pos.y % 106 + 371;
    sfSprite_setOrigin(bg->sprite, pos);
}

static void update_clouds(cam_t *cam)
{
    sprite_t *sprite = *get_spritelist();

    while (sprite != NULL) {
        if (sprite->type == CLOUD && get_tween(sprite->name) == NULL) {
            sprite->angle = diceroll(0, 360);
            sprite->pos.x = 800 + sprite->rect.height + sprite->rect.width;
            sprite->pos.y = diceroll(-50, 650);
            make_tween(sprite->name, &sprite->pos.x, -(sprite->rect.height
            + sprite->rect.width), diceroll(20, 40))->method = LINEAR;
        }
        sprite = sprite->next;
    }
}

void update_technicalstuff(world_t *world)
{
    update_timers();
    update_tweens();
    update_music();
    update_sounds();
    sfRenderWindow_setTitle(WINDOW, world->name);
}

void update_background(cam_t *cam)
{
    sfRenderWindow_clear(WINDOW, (sfColor){255, 230, 200});
    update_bg();
    update_clouds(cam);
    draw_allsprites(CLOUD);
    draw_allsprites(BG);
}

void update_render(world_t *world, sfVertex **vertex,
    sfVertexArray *tile, cam_t *cam)
{
    update_vertexes(world, vertex, cam);
    if (GAMESTUFF->curs_state == MINIMAP)
        vertex_indicate(world, vertex, cam);
    else
        get_sprite("arrow")->draw = 0;
    sfVertexArray_setPrimitiveType(tile, sfQuads);
    update_tiles(world, vertex, tile, cam);
}

void update_hud(world_t *world, sfVertex **vertex,
    sfVertexArray *tile)
{
    draw_allsprites(HUD);
    update_minimap(world, vertex, tile);
    if (!GAMESTUFF->fullscreen && GAMESTUFF->act == RAISE)
        raise_ui();
    if (!GAMESTUFF->fullscreen && GAMESTUFF->act == PAINT)
        paint_ui();
    if (!GAMESTUFF->fullscreen && GAMESTUFF->act == PLACE)
        place_ui();
    draw_alltexts();
}
