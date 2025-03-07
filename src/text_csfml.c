/*
** EPITECH PROJECT, 2024
** text_csfml
** File description:
** Text related functions.
*/

#include "../include/header_csfml.h"

text_t **get_textlist(void)
{
    static text_t *textlist = NULL;

    return &textlist;
}

text_t *get_text(char const *name)
{
    text_t *text = *get_textlist();

    while (text != NULL) {
        if (strcmp(text->name, name) == 0)
            return text;
        text = text->next;
    }
    return NULL;
}

static void make_text_other(text_t *text)
{
    if (*get_textlist() == NULL)
        text->font = sfFont_createFromFile("assets/fonts/VT323-Regular.ttf");
    else
        text->font = (*get_textlist())->font;
    sfText_setCharacterSize(text->text, 60);
}

text_t *make_text(char *name, char *str, int x, int y)
{
    text_t *text = malloc(sizeof(text_t));

    if (text == NULL)
        return NULL;
    text->name = strdup(name);
    text->text = sfText_create();
    make_text_other(text);
    sfText_setFont(text->text, text->font);
    text->str = strdup(str);
    sfText_setString(text->text, text->str);
    text->pos = (sfVector2f){x, y};
    text->scale = (sfVector2f){1, 1};
    text->angle = 0;
    text->alpha = 1;
    text->color = sfBlack;
    text->draw = 1;
    text->next = *get_textlist();
    *get_textlist() = text;
    return text;
}

void draw_alltexts(void)
{
    text_t *text = *get_textlist();

    while (text != NULL) {
        if (text->draw) {
            sfText_setPosition(text->text, text->pos);
            sfText_setScale(text->text, text->scale);
            sfText_setRotation(text->text, text->angle);
            text->color.a = text->alpha * 255;
            sfText_setFillColor(text->text, text->color);
            sfRenderWindow_drawText(WINDOW, text->text, NULL);
        }
        text = text->next;
    }
}

static void free_text(text_t *text)
{
    sfText_destroy(text->text);
    free(text->name);
    free(text->str);
    free(text);
}

static void destroy_firsttext(void)
{
    text_t *next = NULL;

    if (*get_textlist() == NULL)
        return;
    next = (*get_textlist())->next;
    free_text(*get_textlist());
    *get_textlist() = next;
}

static void destroy_lasttext(void)
{
    text_t *list = *get_textlist();

    if (list->next == NULL) {
        free_text(*get_textlist());
        return;
    }
    while (list->next->next != NULL)
        list = list->next;
    free_text(list->next);
    list->next = NULL;
}

void destroy_text(text_t *text)
{
    text_t *list = *get_textlist();
    text_t *tmp = NULL;

    if (strcmp(list->name, text->name) == 0) {
        destroy_firsttext();
        return;
    }
    while (list->next != NULL) {
        if (strcmp(list->next->name, text->name) == 0)
            break;
        list = list->next;
    }
    if (list->next == NULL) {
        destroy_lasttext();
        return;
    }
    tmp = list->next;
    list->next = tmp->next;
    free_text(tmp);
}
