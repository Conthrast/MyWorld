/*
** EPITECH PROJECT, 2024
** header
** File description:
** Header for the MyWorld project.
*/

#ifndef MYWORLD_H
    #define MYWORLD_H

    #include "header_csfml.h"
    #include <unistd.h>
    #include <dirent.h>
    #include <stdio.h>
    #include <time.h>

    #define GAMESTUFF (*get_gamestuff())
    #define ITEM (*get_gamestuff())->item
    #define STATE (*get_renderstate())
    #define SQ(x) pow(x, 2)

    #define HEIGHT_MAX 60.0
    #define HEIGHT_MIN -30.0
    #define INTENSITY 30

typedef enum user_actions {
    RAISE,
    PAINT,
    PLACE
} act_t;

typedef enum cursor_states {
    NOSTATE,
    CAMERA,
    MINIMAP
} cursor_t;

typedef struct game {
    act_t act;
    int area;
    sprite_t *item;
    sfVector2i curs_coords;
    cursor_t curs_state;
    int fullscreen;
} game_t;

typedef struct world {
    char *name;
    int size;
    float **map;
    sprite_t ***objects;
    sprite_t ***textures;
} world_t;

typedef struct camera {
    sfVector2f offset;
    float rotation;
    float tilt;
    float zoom;
    float zoomdef;
} cam_t;

void init_assets(void);
void destroy_assets(cam_t *cam);
cam_t *init_cam(int size);
sfVertex **init_vertexes(world_t *world);

void update_vertexes(world_t *world, sfVertex **vertex, cam_t *cam);
void vertex_indicate(world_t *world,
    sfVertex **vertex, cam_t *cam);

void set_texture(int x, int y, world_t *world, int angle);
void update_vertexcolor(int x, int y, world_t *world, sfVertex **vertex);
void update_tiles(world_t *world, sfVertex **vertex,
    sfVertexArray *tile, cam_t *cam);

void update_minimap(world_t *world, sfVertex **vertex, sfVertexArray *tile);

void raise_ui(void);
void paint_ui(void);
void place_ui(void);

void update_technicalstuff(world_t *world);
void update_background(cam_t *cam);
void update_render(world_t *world, sfVertex **vertex,
    sfVertexArray *tile, cam_t *cam);
void update_hud(world_t *world, sfVertex **vertex,
    sfVertexArray *tile);

int save_world_prompt(world_t *world);
int load_world_prompt(world_t **world, sfVertex ***vertex, cam_t **cam);
int init_world_prompt(world_t **world, sfVertex ***vertex, cam_t **cam);
void game_hotkeys(sfKeyCode key, world_t **world,
    sfVertex ***vertex, cam_t **cam);

void cam_reset(cam_t *cam);
void cam_interaction(sfEvent event, cam_t *cam);

void vertex_interaction(sfEvent event, world_t *world,
    sfVertex **vertex, cam_t *cam);

int diceroll(int low, int high);
void play_random_sound(char *name, int range, float volume, float pitch);
void animate(sprite_t *sprite, int first, int last, int fps);
char *int_to_str(int nbr);
void text_jumpscare(char *str, float dur);

char *prompt_txt(char const *prompt, char *pre);
int prompt_nbr(char const *prompt, int pre, int min, int max);

world_t *init_world(int size);
void destroy_world(world_t *world, sfVertex **vertex);
void clear_world(world_t *world, sfVertex **vertex, cam_t *cam);
world_t *load_world(char *name);
void save_world(world_t *world);
int valid_world(char *name);

sfRenderStates **get_renderstate(void);
void init_renderstate(void);

game_t **get_gamestuff(void);
game_t *init_gamestuff(void);

#endif /* MYWORLD_H */
