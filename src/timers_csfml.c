/*
** EPITECH PROJECT, 2024
** timers_csfml
** File description:
** Timer related functions.
*/

#include "../include/header_csfml.h"

timers_t **get_timerlist(void)
{
    static timers_t *timerlist = NULL;

    return &timerlist;
}

timers_t *get_timer(char const *name)
{
    timers_t *timer = *get_timerlist();

    while (timer != NULL) {
        if (strcmp(timer->name, name) == 0)
            return timer;
        timer = timer->next;
    }
    return NULL;
}

timers_t *run_timer(char *name, float dur)
{
    timers_t *timer = malloc(sizeof(timers_t));

    if (timer == NULL)
        return NULL;
    if (get_timer(name) != NULL)
        destroy_timer(name);
    timer->name = strdup(name);
    timer->tend = TIME + dur;
    timer->next = *get_timerlist();
    *get_timerlist() = timer;
    return timer;
}

void update_timers(void)
{
    timers_t *timer = *get_timerlist();
    timers_t *tmp;

    while (timer != NULL) {
        tmp = timer->next;
        if (TIME >= timer->tend)
            destroy_timer(timer->name);
        timer = tmp;
    }
}

static void free_timer(timers_t *timer)
{
    free(timer->name);
    free(timer);
}

static void destroy_firsttimer(void)
{
    timers_t *next = NULL;

    if (*get_timerlist() == NULL)
        return;
    next = (*get_timerlist())->next;
    free_timer(*get_timerlist());
    *get_timerlist() = next;
}

static void destroy_lasttimer(void)
{
    timers_t *list = *get_timerlist();

    if (list->next == NULL) {
        free_timer(*get_timerlist());
        return;
    }
    while (list->next->next != NULL)
        list = list->next;
    free_timer(list->next);
    list->next = NULL;
}

void destroy_timer(char const *name)
{
    timers_t *list = *get_timerlist();
    timers_t *tmp = NULL;

    if (strcmp(list->name, name) == 0) {
        destroy_firsttimer();
        return;
    }
    while (list->next != NULL) {
        if (strcmp(list->next->name, name) == 0)
            break;
        list = list->next;
    }
    if (list->next == NULL) {
        destroy_lasttimer();
        return;
    }
    tmp = list->next;
    list->next = tmp->next;
    free_timer(tmp);
}
