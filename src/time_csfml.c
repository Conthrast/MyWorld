/*
** EPITECH PROJECT, 2024
** time_csfml
** File description:
** Time related functions.
*/

#include "../include/header_csfml.h"

sfClock **get_clock(void)
{
    static sfClock *clock;

    return &clock;
}

float get_time(void)
{
    float seconds = sfClock_getElapsedTime(*get_clock()).microseconds
    / 1000000.0;

    return seconds;
}

void destroy_clock(void)
{
    sfClock_destroy(*get_clock());
}
