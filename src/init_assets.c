/*
** EPITECH PROJECT, 2024
** init_assets
** File description:
** Functions to initialize the program's assets.
*/

#include "../include/header.h"

static void init_spritegroup_makesprite(char const *path, type_t type,
    char *fullpath, struct dirent *buff)
{
    sprite_t *sprite;

    strcpy(fullpath, path);
    strcat(fullpath, buff->d_name);
    sprite = make_sprite(buff->d_name, fullpath, 0, 0);
    sprite->type = type;
    free(fullpath);
}

static void init_spritegroup(char const *path, type_t type)
{
    DIR *dr = opendir(path);
    struct dirent *buff;
    char *fullpath;

    buff = readdir(dr);
    while (buff != NULL) {
        if (strcmp(&(buff->d_name)[strlen(buff->d_name) - 4], ".png") != 0) {
            buff = readdir(dr);
            continue;
        }
        fullpath = malloc(sizeof(char) *
        (strlen(path) + strlen(buff->d_name) + 1));
        if (fullpath == NULL) {
            closedir(dr);
            return;
        }
        init_spritegroup_makesprite(path, type, fullpath, buff);
        buff = readdir(dr);
    }
    closedir(dr);
}

static void init_clouds(void)
{
    sprite_t *sprite = *get_spritelist();

    get_sprite("nuage.png")->scale = (sfVector2f){2, 2};
    get_sprite("dragon_ball_cloud.png")->scale = (sfVector2f){2, 2};
    while (sprite != NULL) {
        if (sprite->type == CLOUD) {
            sfSprite_setOrigin(sprite->sprite,
            (sfVector2f){sprite->rect.width / 2.0, sprite->rect.height / 2.0});
            sprite->color = (sfColor){255, 255, 255, 50};
            sprite->pos.x = 5000;
            make_tween(sprite->name, &(sprite->pos.x), 5000,
            diceroll(0, 30));
        }
        sprite = sprite->next;
    }
}

void init_sprites(void)
{
    make_sprite("arrow", "assets/arrow.png", 400, 300);
    make_sprite("bg", "assets/BG.png", 550, 250);
    make_sprite("UI", "assets/UI.png", 0, 0);
    sfSprite_setOrigin(get_sprite("arrow")->sprite, (sfVector2f){8, 40});
    get_sprite("arrow")->scale = (sfVector2f){2.0, 2.0};
    get_sprite("arrow")->rect.width = 16;
    get_sprite("arrow")->type = OTHER;
    get_sprite("bg")->scale = (sfVector2f){0.7, 0.7};
    get_sprite("bg")->color = (sfColor){255, 255, 255, 75};
    get_sprite("bg")->type = BG;
    get_sprite("UI")->scale = (sfVector2f){2, 2};
    get_sprite("UI")->type = HUD;
    init_spritegroup("assets/clouds/", CLOUD);
    init_clouds();
    init_spritegroup("assets/objects/", OBJECT);
    get_sprite("Renault.png")->rect.width = 70;
    get_sprite("the world.png")->rect.width = 65;
    get_sprite("the world.png")->rect.height = 65;
    init_spritegroup("assets/tiles/", TEXTURE);
}

static void init_sounds_part2(void)
{
    make_sound("remove0", "assets/sounds/remove0.wav");
    make_sound("remove1", "assets/sounds/remove1.wav");
    make_sound("remove2", "assets/sounds/remove2.wav");
    make_sound("goofy0", "assets/sounds/goofy0.wav");
    make_sound("goofy1", "assets/sounds/goofy1.wav");
    make_sound("goofy2", "assets/sounds/goofy2.wav");
    make_sound("goofy3", "assets/sounds/goofy3.wav");
    make_sound("goofy4", "assets/sounds/goofy4.wav");
    make_sound("goofy5", "assets/sounds/goofy5.wav");
    make_sound("goofy6", "assets/sounds/goofy6.wav");
    make_sound("goofy7", "assets/sounds/goofy7.wav");
}

void init_sounds(void)
{
    make_sound("worldnew", "assets/sounds/world new.wav");
    make_sound("worldsave", "assets/sounds/world save.wav");
    make_sound("worldload", "assets/sounds/world load.wav");
    make_sound("raise", "assets/sounds/raise.wav");
    make_sound("paint", "assets/sounds/paint.wav");
    make_sound("erase", "assets/sounds/paint erase.wav");
    make_sound("place", "assets/sounds/place.wav");
    make_sound("camspin", "assets/sounds/cam spin.wav");
    make_sound("camup", "assets/sounds/cam tilt up.wav");
    make_sound("camdown", "assets/sounds/cam tilt down.wav");
    make_sound("cambuzz", "assets/sounds/cam buzz.wav");
    make_sound("reset", "assets/sounds/reset.wav");
    make_sound("fs", "assets/sounds/fullscreen.wav");
    make_sound("areaup", "assets/sounds/area up.wav");
    make_sound("areadown", "assets/sounds/area down.wav");
    make_sound("areadown", "assets/sounds/area down.wav");
    make_sound("areabuzz", "assets/sounds/area buzz.wav");
    make_sound("switch", "assets/sounds/act switch.wav");
    make_sound("item", "assets/sounds/item switch.wav");
    init_sounds_part2();
}

static void init_music_makemusic(char const *path, struct dirent *buff)
{
    music_t *music;
    char *fullpath = malloc(sizeof(char) *
    (strlen(path) + strlen(buff->d_name) + 1));

    if (fullpath == NULL)
        return;
    strcpy(fullpath, path);
    strcat(fullpath, buff->d_name);
    music = play_music(buff->d_name, fullpath, 0, 0);
    sfMusic_setLoop(music->music, sfTrue);
    make_tween("volintro", &music->volume, 75, 7)->method = EASEOUT;
    make_tween("pitchintro", &music->pitch, 1.0, 7)->method = EASEOUT;
    free(fullpath);
}

void init_music(void)
{
    DIR *dr;
    struct dirent *buff;
    char path[] = "assets/music/";

    dr = opendir(path);
    buff = readdir(dr);
    while (strcmp(&(buff->d_name)[strlen(buff->d_name) - 4], ".ogg") != 0) {
        buff = readdir(dr);
        if (buff == NULL) {
            closedir(dr);
            return;
        }
        continue;
    }
    init_music_makemusic(path, buff);
    closedir(dr);
}

void init_assets(void)
{
    create_window(800, 600, "MyWorld");
    *get_clock() = sfClock_create();
    init_sprites();
    init_sounds();
    init_music();
}

void destroy_assets(cam_t *cam)
{
    free(cam);
    free(GAMESTUFF);
    free(STATE);
    while (*get_spritelist())
        destroy_sprite(*get_spritelist());
    if (*get_textlist() != NULL)
        sfFont_destroy((*get_textlist())->font);
    while (*get_textlist())
        destroy_text(*get_textlist());
    while (*get_tweenlist())
        destroy_tween((*get_tweenlist())->name);
    while (*get_timerlist())
        destroy_timer((*get_timerlist())->name);
    while (*get_soundbank())
        destroy_sound(*get_soundbank());
    destroy_music();
    destroy_clock();
    destroy_window();
}
