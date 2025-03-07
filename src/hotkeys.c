/*
** EPITECH PROJECT, 2024
** hotkeys
** File description:
** Functions that manage all the different keypress events.
*/

#include "../include/header.h"

static void toggle_fullscreen_effects(cam_t *cam, int size)
{
    sprite_t *bg = get_sprite("bg");

    if (GAMESTUFF->fullscreen) {
        cam->zoom *= 1.4;
        cam->zoomdef = 400 / size * 1.4;
        bg->pos.x = 400;
        bg->pos.y = 300;
        get_sprite("UI")->draw = 0;
        play_sound("fs", 100, diceroll(120, 130) / 100.0);
    } else {
        cam->zoom /= 1.4;
        cam->zoomdef = 400 / size;
        bg->pos.x = 550;
        bg->pos.y = 250;
        get_sprite("UI")->draw = 1;
        play_sound("fs", 100, diceroll(70, 80) / 100.0);
    }
}

static void toggle_fullscreen(cam_t *cam, int size)
{
    GAMESTUFF->fullscreen = 1 - GAMESTUFF->fullscreen;
    if (get_tween("camzoom") != NULL) {
        cam->zoom = get_tween("camzoom")->dest;
        destroy_tween("camzoom");
    }
    toggle_fullscreen_effects(cam, size);
}

static void find_next_item_oftype(sprite_t *item, type_t type)
{
    sprite_t *sprite = *get_spritelist();
    int found = 0;

    while (sprite != NULL) {
        if (found && sprite->type == type) {
            GAMESTUFF->item = sprite;
            return;
        }
        if (sprite == item)
            found = 1;
        sprite = sprite->next;
    }
    sprite = *get_spritelist();
    while (sprite != NULL) {
        if (sprite->type == type) {
            GAMESTUFF->item = sprite;
            return;
        }
        sprite = sprite->next;
    }
}

void select_next_item(void)
{
    sprite_t *item = GAMESTUFF->item;
    type_t type;

    if (GAMESTUFF->act == PAINT)
        type = TEXTURE;
    else if (GAMESTUFF->act == PLACE)
        type = OBJECT;
    else
        return;
    find_next_item_oftype(item, type);
}

static void hotkeys_toolbar(sfKeyCode key)
{
    if (key == sfKeyW && GAMESTUFF->act != RAISE) {
        play_sound("item", diceroll(30, 45), diceroll(90, 110) / 100.0);
        select_next_item();
    }
    if (key == sfKeySpace) {
        play_sound("switch", diceroll(70, 75), 1.0);
        GAMESTUFF->act = (GAMESTUFF->act + 1) % 3;
        select_next_item();
    }
}

static void hotkeys_area(sfKeyCode key, world_t *world)
{
    int area = GAMESTUFF->area;

    if (key == sfKeyAdd || key == sfKeyP) {
        if (area >= world->size) {
            play_sound("areabuzz", 75, 1.0);
        } else {
            play_sound("areaup", 75, 1.0 + area / 10.0);
            GAMESTUFF->area++;
        }
    }
    if (key == sfKeySubtract || key == sfKeyM) {
        if (area <= 0) {
            play_sound("areabuzz", 75, 1.0);
        } else {
            play_sound("areadown", 75, 0.5 + area / 10.0);
            GAMESTUFF->area--;
        }
    }
}

int save_world_prompt(world_t *world)
{
    char *str = prompt_txt("Save as:", world->name);

    if (str == NULL)
        return 0;
    free(world->name);
    world->name = str;
    save_world(world);
    text_jumpscare("World saved!", 3);
    play_sound("worldsave", 75, 1.0);
    return 1;
}

int load_world_prompt(world_t **world, sfVertex ***vertex, cam_t **cam)
{
    char *str = prompt_txt("Load file:", "");

    if (str == NULL)
        return 0;
    if (!valid_world(str)) {
        text_jumpscare("Invalid file name!", 2);
        play_sound("cambuzz", 75, 1.0);
        return -1;
    }
    clear_world(*world, *vertex, *cam);
    *world = load_world(str);
    *cam = init_cam((*world)->size);
    *vertex = init_vertexes(*world);
    free(str);
    text_jumpscare("World loaded!", 3);
    play_sound("worldload", 75, 1.0);
    return 1;
}

int init_world_prompt(world_t **world, sfVertex ***vertex, cam_t **cam)
{
    int size = prompt_nbr("New world size:", (*world)->size, 4, 128);

    if (size == -1)
        return 0;
    clear_world(*world, *vertex, *cam);
    *world = init_world(size);
    *cam = init_cam((*world)->size);
    *vertex = init_vertexes(*world);
    play_sound("worldnew", 75, 1.0);
    return 1;
}

void game_hotkeys(sfKeyCode key, world_t **world,
    sfVertex ***vertex, cam_t **cam)
{
    if (key == sfKeyS)
        save_world_prompt(*world);
    if (key == sfKeyL)
        load_world_prompt(world, vertex, cam);
    if (key == sfKeyN)
        init_world_prompt(world, vertex, cam);
    hotkeys_toolbar(key);
    hotkeys_area(key, *world);
    if (key == sfKeyTab)
        toggle_fullscreen(*cam, (*world)->size);
    if (key == sfKeyEscape)
        sfRenderWindow_close(WINDOW);
}
