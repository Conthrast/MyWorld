/*
** EPITECH PROJECT, 2024
** tweens_csfml
** File description:
** Tween related functions.
*/

#include "../include/header_csfml.h"

tween_t **get_tweenlist(void)
{
    static tween_t *tweenlist = NULL;

    return &tweenlist;
}

tween_t *get_tween(char const *name)
{
    tween_t *tween = *get_tweenlist();

    while (tween != NULL) {
        if (strcmp(tween->name, name) == 0)
            return tween;
        tween = tween->next;
    }
    return NULL;
}

static void do_tween(tween_t *tween, method_t method)
{
    float *val = tween->value;
    double prog = (TIME - tween->tstart) / (tween->tend - tween->tstart);
    double fact;

    if (method == LINEAR) {
        *val = tween->start + ((tween->dest - tween->start) /
        (tween->tend - tween->tstart)) * (TIME - tween->tstart);
        return;
    }
    if (method == EASEIN)
        fact = pow(prog, 5);
    if (method == EASEOUT)
        fact = 1 - pow(1 - prog, 5);
    if (method == EASEINOUT) {
        if (prog < 0.5)
            fact = 2 * pow(prog, 2);
        else
            fact = (1 - pow(-2 * prog + 2, 2) / 2);
    }
    *val = tween->start + (tween->dest - tween->start) * fact;
}

void update_tweens(void)
{
    tween_t *tween = *get_tweenlist();
    tween_t *tmp = NULL;

    while (tween != NULL) {
        tmp = tween->next;
        do_tween(tween, tween->method);
        if (TIME >= tween->tend) {
            *tween->value = tween->dest;
            destroy_tween(tween->name);
        }
        tween = tmp;
    }
}

tween_t *make_tween(char *name, float *val, float dest, float dur)
{
    tween_t *tween = malloc(sizeof(tween_t));

    if (tween == NULL)
        return NULL;
    if (get_tween(name) != NULL)
        destroy_tween(name);
    tween->name = strdup(name);
    tween->value = val;
    tween->start = *val;
    tween->dest = dest;
    tween->tstart = TIME;
    tween->tend = TIME + dur;
    tween->method = LINEAR;
    tween->next = *get_tweenlist();
    *get_tweenlist() = tween;
    return tween;
}

static void free_tween(tween_t *tween)
{
    free(tween->name);
    free(tween);
}

static void destroy_firsttween(void)
{
    tween_t *next = NULL;

    if (*get_tweenlist() == NULL)
        return;
    next = (*get_tweenlist())->next;
    free_tween(*get_tweenlist());
    *get_tweenlist() = next;
}

static void destroy_lasttween(void)
{
    tween_t *list = *get_tweenlist();

    if (list->next == NULL) {
        free_tween(*get_tweenlist());
        return;
    }
    while (list->next->next != NULL)
        list = list->next;
    free_tween(list->next);
    list->next = NULL;
}

void destroy_tween(char const *name)
{
    tween_t *list = *get_tweenlist();
    tween_t *tmp = NULL;

    if (strcmp(list->name, name) == 0) {
        destroy_firsttween();
        return;
    }
    while (list->next != NULL) {
        if (strcmp(list->next->name, name) == 0)
            break;
        list = list->next;
    }
    if (list->next == NULL) {
        destroy_lasttween();
        return;
    }
    tmp = list->next;
    list->next = tmp->next;
    free_tween(tmp);
}
