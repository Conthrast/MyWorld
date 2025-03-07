/*
** EPITECH PROJECT, 2024
** prompt_txt
** File description:
** Functions for prompting text input.
*/

#include "../../include/header.h"

static sfRenderWindow *prompt_txt_createwindow(char const *title)
{
    sfRenderWindow *window;
    sfVideoMode video_mode;

    video_mode.width = 300;
    video_mode.height = 100;
    video_mode.bitsPerPixel = 32;
    window = sfRenderWindow_create(video_mode, title, sfDefaultStyle, NULL);
    sfRenderWindow_setFramerateLimit(window, 30);
    return window;
}

static void prompt_txt_updatetext(char *str, sfText *txt)
{
    if (str[0] == '\0') {
        sfText_setString(txt, "enter text here...");
        sfText_setFillColor(txt, (sfColor){0, 0, 0, 100});
    } else {
        sfText_setString(txt, str);
        sfText_setFillColor(txt, sfBlack);
    }
}

static void prompt_txt_cancel(char **str, sfRenderWindow *window)
{
    free(*str);
    *str = NULL;
    sfRenderWindow_close(window);
}

static void prompt_txt_trackinput(sfKeyCode key, char **str, sfText *txt,
    sfRenderWindow *window)
{
    char ch = key + 'a';
    int i = 0;

    if (key == sfKeyEnter) {
        if (*str[0] == '\0')
            return prompt_txt_cancel(str, window);
        sfRenderWindow_close(window);
    }
    if (key == sfKeyEscape)
        return prompt_txt_cancel(str, window);
    while ((*str)[i] != '\0')
        i++;
    if (i != 0 && key == sfKeyBackspace)
        (*str)[i - 1] = '\0';
    if (key == sfKeySpace)
        (*str)[i] = ' ';
    if (ch >= 'a' && ch <= 'z' && i != 16)
        (*str)[i] = ch;
    prompt_txt_updatetext(*str, txt);
}

static void prompt_txt_events(char **str, sfText *txt, sfRenderWindow *window)
{
    sfEvent event;

    while (sfRenderWindow_pollEvent(window, &event)) {
        if (event.type == sfEvtKeyPressed)
            prompt_txt_trackinput(event.key.code, str, txt, window);
        if (event.type == sfEvtClosed)
            prompt_txt_cancel(str, window);
    }
}

void prompt_txt_setuptexts(char const *prompt, sfText *prompttxt,
    sfText *txt, sfFont *font)
{
    sfText_setString(prompttxt, prompt);
    sfText_setPosition(prompttxt, (sfVector2f){12, 15});
    sfText_setFont(prompttxt, font);
    sfText_setFillColor(prompttxt, (sfColor){100, 80, 240, 255});
    sfText_setCharacterSize(prompttxt, 23);
    sfText_setPosition(txt, (sfVector2f){12, 45});
    sfText_setFont(txt, font);
    sfText_setFillColor(txt, sfBlack);
    sfText_setCharacterSize(txt, 30);
}

void *prompt_txt_destroy(sfText *prompttxt, sfText *txt, sfFont *font,
    sfRenderWindow *window)
{
    sfText_destroy(prompttxt);
    sfText_destroy(txt);
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);
    return NULL;
}

void prompt_txt_updatewindow(sfText *prompttxt, sfText *txt,
    sfRenderWindow *window)
{
    sfRenderWindow_drawText(window, prompttxt, NULL);
    sfRenderWindow_drawText(window, txt, NULL);
    sfRenderWindow_display(window);
}

char *prompt_txt(char const *prompt, char *pre)
{
    sfRenderWindow *window = prompt_txt_createwindow("Text input prompt");
    sfFont *font = sfFont_createFromFile("assets/fonts/OpenSans-Regular.ttf");
    sfText *prompttxt = sfText_create();
    sfText *txt = sfText_create();
    char *str = malloc(sizeof(char) * 17);

    if (str == NULL)
        return prompt_txt_destroy(prompttxt, txt, font, window);
    strcpy(str, pre);
    prompt_txt_setuptexts(prompt, prompttxt, txt, font);
    for (int i = strlen(pre); i < 17; i++)
        str[i] = '\0';
    prompt_txt_updatetext(str, txt);
    while (sfRenderWindow_isOpen(window)) {
        sfRenderWindow_clear(window, sfWhite);
        prompt_txt_events(&str, txt, window);
        prompt_txt_updatewindow(prompttxt, txt, window);
    }
    prompt_txt_destroy(prompttxt, txt, font, window);
    return str;
}
