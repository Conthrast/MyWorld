/*
** EPITECH PROJECT, 2024
** prompt_nbr
** File description:
** Functions for prompting number input.
*/

#include "../../include/header.h"

static sfRenderWindow *prompt_nbr_createwindow(char const *title)
{
    sfRenderWindow *window;
    sfVideoMode video_mode;

    video_mode.width = 300;
    video_mode.height = 100;
    video_mode.bitsPerPixel = 32;
    window = sfRenderWindow_create(video_mode, title, sfDefaultStyle, NULL);
    sfRenderWindow_setFramerateLimit(window, 60);
    return window;
}

static void prompt_nbr_updaterect(int nbr, int lim[2], sfText *val,
    sfRectangleShape *bar)
{
    float min = lim[0];
    float max = lim[1];
    float prog = (nbr - min) / (max - min);
    sfColor color = {(cos(TIME) + 1) * prog * 50 + 155, (cos(TIME + 2) + 1)
    * prog * 50 + 155, (cos(TIME + 4.1) + 1) * prog * 50 + 155, 255};
    char *str;

    sfText_setPosition(val, (sfVector2f){210 + diceroll(-2, 2) * pow(prog, 10),
    11 + diceroll(-2, 2) * pow(prog, 10)});
    if (nbr == -1)
        return;
    str = int_to_str(nbr);
    sfText_setString(val, str);
    sfRectangleShape_setSize(bar,
    (sfVector2f){prog * 268.0, 11});
    sfRectangleShape_setFillColor(bar, color);
    color = (sfColor){color.r / 1.2, color.g / 1.2, color.b / 1.2, 255};
    sfRectangleShape_setOutlineColor(bar, color);
    free(str);
}

static void prompt_nbr_cancel(int *nbr, sfRenderWindow *window)
{
    *nbr = -1;
    sfRenderWindow_close(window);
}

static void prompt_nbr_trackinput(int *nbr, int min, int max,
    sfRenderWindow *window)
{
    sfVector2i mouse = sfMouse_getPositionRenderWindow(window);

    if (mouse.x < 10 || mouse.x > 290 || mouse.y < 58 || mouse.y > 81)
        return;
    if (MOUSEPRESS(sfMouseLeft))
        *nbr = ((mouse.x - 14) / 272.0) * (max - min) + min + 0.5;
    *nbr = MIN(MAX(*nbr, min), max);
}

static void prompt_nbr_events(int *nbr, int min, int max,
    sfRenderWindow *window)
{
    sfEvent event;

    prompt_nbr_trackinput(nbr, min, max, window);
    while (sfRenderWindow_pollEvent(window, &event)) {
        if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEnter)
            sfRenderWindow_close(window);
        if (event.type == sfEvtClosed ||
        (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape))
            prompt_nbr_cancel(nbr, window);
    }
}

void prompt_nbr_setuptexts(char const *prompt, sfText *prompttxt,
    sfText *val, sfFont *font)
{
    sfText_setString(prompttxt, prompt);
    sfText_setPosition(prompttxt, (sfVector2f){12, 15});
    sfText_setFont(prompttxt, font);
    sfText_setFillColor(prompttxt, (sfColor){100, 80, 240, 255});
    sfText_setCharacterSize(prompttxt, 23);
    sfText_setPosition(val, (sfVector2f){210, 11});
    sfText_setFont(val, font);
    sfText_setFillColor(val, sfBlack);
    sfText_setCharacterSize(val, 30);
}

void prompt_nbr_setuprects(sfRectangleShape *box, sfRectangleShape *bar)
{
    sfRectangleShape_setPosition(box, (sfVector2f){14, 62});
    sfRectangleShape_setSize(box, (sfVector2f){272, 15});
    sfRectangleShape_setFillColor(box, (sfColor){0, 0, 0, 25});
    sfRectangleShape_setOutlineColor(box, (sfColor){0, 0, 0, 180});
    sfRectangleShape_setOutlineThickness(box, 2);
    sfRectangleShape_setPosition(bar, (sfVector2f){16, 64});
    sfRectangleShape_setSize(bar, (sfVector2f){146, 11});
    sfRectangleShape_setFillColor(bar, sfRed);
    sfRectangleShape_setOutlineColor(bar, sfBlue);
    sfRectangleShape_setOutlineThickness(bar, 2);
}

void *prompt_nbr_destroy(sfText *txt[2], sfRectangleShape *rect[2],
    sfFont *font, sfRenderWindow *window)
{
    sfText_destroy(txt[0]);
    sfText_destroy(txt[1]);
    sfRectangleShape_destroy(rect[0]);
    sfRectangleShape_destroy(rect[1]);
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);
    return NULL;
}

void prompt_nbr_updatewindow(sfText *prompttxt, sfText *val,
    sfRectangleShape *rect[2], sfRenderWindow *window)
{
    sfRenderWindow_drawText(window, prompttxt, NULL);
    sfRenderWindow_drawText(window, val, NULL);
    sfRenderWindow_drawRectangleShape(window, rect[0], NULL);
    sfRenderWindow_drawRectangleShape(window, rect[1], NULL);
    sfRenderWindow_display(window);
}

int prompt_nbr(char const *prompt, int pre, int min, int max)
{
    sfRenderWindow *window = prompt_nbr_createwindow("Number input prompt");
    sfFont *font = sfFont_createFromFile("assets/fonts/OpenSans-Regular.ttf");
    sfText *prompttxt = sfText_create();
    sfText *val = sfText_create();
    sfRectangleShape *box = sfRectangleShape_create();
    sfRectangleShape *bar = sfRectangleShape_create();
    int nbr = pre;

    prompt_nbr_setuptexts(prompt, prompttxt, val, font);
    prompt_nbr_setuprects(box, bar);
    while (sfRenderWindow_isOpen(window)) {
        sfRenderWindow_clear(window, sfWhite);
        prompt_nbr_events(&nbr, min, max, window);
        prompt_nbr_updaterect(nbr, (int[2]){min, max}, val, bar);
        prompt_nbr_updatewindow(prompttxt, val,
        (sfRectangleShape *[2]){box, bar}, window);
    }
    prompt_nbr_destroy((sfText *[2]){prompttxt, val},
    (sfRectangleShape *[2]){box, bar}, font, window);
    return nbr;
}
