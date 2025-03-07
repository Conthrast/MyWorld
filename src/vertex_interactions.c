/*
** EPITECH PROJECT, 2024
** vertex_interactions
** File description:
** Vertex interaction related functions.
*/

#include "../include/header.h"

void vertex_track(int size)
{
    sfVector2i mouse = sfMouse_getPositionRenderWindow(WINDOW);
    float minimapsize = 250;
    int x;
    int y;

    if (GAMESTUFF->curs_state == CAMERA || GAMESTUFF->fullscreen
    || mouse.x < 0 || mouse.x > 250 || mouse.y < 30 || mouse.y > 280) {
        if (GAMESTUFF->curs_state == MINIMAP)
            GAMESTUFF->curs_state = NOSTATE;
        get_sprite("arrow")->draw = 0;
        return;
    }
    x = (mouse.x / minimapsize) * (size - 1) + 0.5;
    y = ((mouse.y - 30) / minimapsize) * (size - 1) + 0.5;
    GAMESTUFF->curs_state = MINIMAP;
    GAMESTUFF->curs_coords = (sfVector2i){x, y};
}

static void vertex_raise(int x, int y, int polarity, world_t *world)
{
    world->map[y][x] += polarity * 0.2 - 0.1;
    if (world->map[y][x] > HEIGHT_MAX)
        world->map[y][x] = HEIGHT_MAX;
    if (world->map[y][x] < HEIGHT_MIN)
        world->map[y][x] = HEIGHT_MIN;
}

static void vertex_paint(int x, int y, int polarity, world_t *world)
{
    if (world->textures[y][x] != ITEM && polarity) {
        world->textures[y][x] = ITEM;
        if (get_sound("paint")->status == 0 || get_sound("paint")->time > 0.2)
            play_sound("paint", 60, 0.9 + cos(TIME) / 30.0);
    }
    if (world->textures[y][x] != NULL && !polarity) {
        world->textures[y][x] = NULL;
        if (get_sound("erase")->status == 0)
            play_sound("erase", 50, diceroll(70, 130) / 100.0);
    }
}

static void vertex_place(int x, int y, int polarity, world_t *world)
{
    if (world->objects[y][x] != ITEM && polarity) {
        world->objects[y][x] = ITEM;
        if (!get_timer("place")) {
            run_timer("place", 0.1);
            play_sound("place", 90, diceroll(50, 140) / 100.0);
        }
    }
    if (world->objects[y][x] != NULL && !polarity) {
        world->objects[y][x] = NULL;
        if (!get_timer("remove")) {
            run_timer("remove", 0.5);
            play_random_sound("remove", 2, 35, diceroll(70, 130) / 100.0);
            play_random_sound("goofy", 7, 20, diceroll(90, 110) / 100.0);
        }
    }
}

static void vertex_interact(int x, int y, int polarity, world_t *world)
{
    if (GAMESTUFF->act == RAISE)
        vertex_raise(x, y, polarity, world);
    if (GAMESTUFF->act == PAINT)
        vertex_paint(MIN(x, world->size - 2), MIN(y, world->size - 2),
        polarity, world);
    if (GAMESTUFF->act == PLACE)
        vertex_place(MIN(MAX(x, 1), world->size - 2),
        MIN(MAX(y, 1), world->size - 2), polarity, world);
}

static void vertex_area_effect(world_t *world)
{
    sfVector2i curs = GAMESTUFF->curs_coords;
    int polarity = MOUSEPRESS(sfMouseLeft);
    int area = GAMESTUFF->area;
    int x = curs.x;
    int y = curs.y;

    if (!MOUSEPRESS(sfMouseLeft) && !MOUSEPRESS(sfMouseRight))
        return;
    if (GAMESTUFF->act == RAISE && get_sound("raise")->status == 0)
        play_sound("raise", 40, (0.8 + polarity / 10.0)
        + world->map[y][x] / 75.0);
    get_sprite("arrow")->pos.y += 8;
    animate(get_sprite("arrow"), 0, 3, 8);
    for (int i = MAX(0, y - area); i <
    MIN(world->size, y + area + (GAMESTUFF->act != PAINT)); i++) {
        for (int j = MAX(0, x - area); j <
        MIN(world->size, x + area + (GAMESTUFF->act != PAINT)); j++)
            vertex_interact(j, i, polarity, world);
    }
}

void vertex_interaction(sfEvent event, world_t *world,
    sfVertex **vertex, cam_t *cam)
{
    if (event.type == sfEvtMouseMoved)
        vertex_track(world->size);
    if (GAMESTUFF->curs_state == MINIMAP)
        vertex_area_effect(world);
}
