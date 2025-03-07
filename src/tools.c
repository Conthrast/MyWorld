/*
** EPITECH PROJECT, 2024
** tools
** File description:
** Tool functions with awesome names.
*/

#include "../include/header.h"

int diceroll(int low, int high)
{
    if (high < low)
        return high;
    return low + rand() % (high - low + 1);
}

void play_random_sound(char *name, int range, float volume, float pitch)
{
    char *newname = malloc(sizeof(char) * (strlen(name) + 2));
    int pick = diceroll(0, range);

    if (newname == NULL)
        return;
    strcpy(newname, name);
    strcat(newname, "0");
    newname[strlen(name)] += pick;
    play_sound(newname, volume, pitch);
    free(newname);
}

void animate(sprite_t *sprite, int first, int last, int fps)
{
    sfIntRect *rect = &sprite->rect;
    sfVector2u size = sfTexture_getSize(sprite->texture);
    unsigned long frame = TIME / (1.0 / fps);
    int frx = size.x / rect->width;
    int fry = size.y / rect->height;

    frame = frame % (last - first + 1) + first;
    rect->left = (frame % frx) * rect->width;
    rect->top = (frame / frx % fry) * rect->height;
}

char *int_to_str(int nbr)
{
    char *str;
    int tmp = nbr;
    int len = 0;

    if (nbr == 0)
        len = 1;
    while (tmp != tmp / 10) {
        tmp /= 10;
        len++;
    }
    str = malloc(sizeof(char) * (len + 1));
    if (str == NULL)
        return NULL;
    for (int i = 0; i < len; i++) {
        str[len - i - 1] = nbr % 10 + '0';
        nbr /= 10;
    }
    str[len] = '\0';
    return str;
}

void text_jumpscare(char *str, float dur)
{
    char *jumpscare[3] = {"jumpscare1", "jumpscare2", "jumpscare3"};
    static int i = 0;
    text_t *txt = get_text(jumpscare[i]);

    if (txt == NULL) {
        txt = make_text(jumpscare[i], str, 400 + diceroll(-30, 30),
        300 + diceroll(-20, 20));
    } else {
        free(txt->str);
        txt->str = strdup(str);
        sfText_setString(txt->text, txt->str);
    }
    sfText_setOrigin(txt->text, (sfVector2f){strlen(str) * 10, 30});
    txt->scale = (sfVector2f){1, 1};
    txt->angle = diceroll(-75, 75) / 10.0;
    txt->alpha = 1;
    txt->color = sfWhite;
    make_tween(jumpscare[i], &txt->alpha, 0, dur)->method = EASEIN;
    i = (i + 1) % 3;
}
