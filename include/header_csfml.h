/*
** EPITECH PROJECT, 2024
** header_csfml
** File description:
** Header for my csfml tools.
*/

#ifndef CSFML_H
    #define CSFML_H

    #include <SFML/Graphics.h>
    #include <SFML/Audio.h>
    #include <SFML/System.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>

    #define WINDOW *get_window()
    #define TIME get_time()
    #define KEYPRESS(key) sfKeyboard_isKeyPressed(key)
    #define MOUSEPRESS(button) sfMouse_isButtonPressed(button)

    #define MIN(a, b) ((a) < (b) ? (a) : (b))
    #define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef enum sprite_types {
    HUD,
    OBJECT,
    TEXTURE,
    CLOUD,
    BG,
    OTHER
} type_t;

typedef enum tween_methods {
    LINEAR,
    EASEIN,
    EASEOUT,
    EASEINOUT
} method_t;

typedef struct sprite {
    char *name;
    sfSprite *sprite;
    sfTexture *texture;
    sfVector2f pos;
    sfVector2f scale;
    float angle;
    sfColor color;
    sfIntRect rect;
    type_t type;
    int draw;
    struct sprite *next;
} sprite_t;

typedef struct text {
    char *name;
    sfText *text;
    sfFont *font;
    char *str;
    sfVector2f pos;
    sfVector2f scale;
    float angle;
    float alpha;
    sfColor color;
    int draw;
    struct text *next;
} text_t;

typedef struct texture {
    char *name;
    sfTexture *texture;
    sfVector2f size;
    struct texture *next;
} texture_t;

typedef struct tween {
    char *name;
    float *value;
    float start;
    float dest;
    float tstart;
    float tend;
    method_t method;
    struct tween *next;
} tween_t;

typedef struct timer {
    char *name;
    float tend;
    struct timer *next;
} timers_t;

typedef struct sound {
    char *name;
    sfSound *sound;
    sfSoundBuffer *buffer;
    float time;
    float volume;
    float pitch;
    sfSoundStatus status;
    struct sound *next;
} sound_t;

typedef struct music {
    char *name;
    sfMusic *music;
    float time;
    float volume;
    float pitch;
} music_t;

sfRenderWindow **get_window(void);
void create_window(unsigned int width, unsigned int height, char const *name);
void destroy_window(void);

sprite_t **get_spritelist(void);
sprite_t *get_sprite(char const *name);
sprite_t *make_sprite(char *name, char const *file, int x, int y);
void draw_sprite(sprite_t *sprite);
void destroy_sprite(sprite_t *sprite);

void draw_allsprites(type_t type);

text_t **get_textlist(void);
text_t *get_text(char const *name);
text_t *make_text(char *name, char *str, int x, int y);
void draw_alltexts(void);
void destroy_text(text_t *text);

texture_t **get_texturelist(void);
texture_t *get_texture(char const *name);
texture_t *make_texture(char *name, char const *file);
void destroy_texture(texture_t *texture);

tween_t **get_tweenlist(void);
tween_t *get_tween(char const *name);
void update_tweens(void);
tween_t *make_tween(char *name, float *val, float dest, float dur);
void destroy_tween(char const *name);

timers_t **get_timerlist(void);
timers_t *get_timer(char const *name);
timers_t *run_timer(char *name, float dur);
void update_timers(void);
void destroy_timer(char const *name);

sound_t *make_sound(char *name, char const *file);

sound_t **get_soundbank(void);
sound_t *get_sound(char const *name);
sound_t *play_sound(char *name, float volume, float pitch);
void update_sounds(void);
void destroy_sound(sound_t *sound);

music_t **get_music(void);
music_t *play_music(char *name, char const *file, float volume, float pitch);
void update_music(void);
void destroy_music(void);

sfClock **get_clock(void);
float get_time(void);
void destroy_clock(void);

#endif /* CSFML_H */
