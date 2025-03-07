/*
** EPITECH PROJECT, 2024
** sounds_csfml
** File description:
** Sound related functions.
*/

#include "../include/header_csfml.h"

sound_t **get_soundbank(void)
{
    static sound_t *soundbank = NULL;

    return &soundbank;
}

sound_t *get_sound(char const *name)
{
    sound_t *sound = *get_soundbank();

    while (sound != NULL) {
        if (strcmp(sound->name, name) == 0)
            return sound;
        sound = sound->next;
    }
    return NULL;
}

sound_t *play_sound(char *name, float volume, float pitch)
{
    sound_t *sound = get_sound(name);

    if (sound == NULL)
        return NULL;
    sound->time = 0.0;
    sound->volume = volume;
    sound->pitch = pitch;
    sfSound_play(sound->sound);
    update_sounds();
    return sound;
}

void update_sounds(void)
{
    sound_t *sound = *get_soundbank();

    while (sound != NULL) {
        sound->time = sfSound_getPlayingOffset(sound->sound).microseconds
        / 1000000.0;
        sfSound_setVolume(sound->sound, sound->volume);
        sfSound_setPitch(sound->sound, sound->pitch);
        sound->status = sfSound_getStatus(sound->sound);
        sound = sound->next;
    }
}

static void free_sound(sound_t *sound)
{
    sfSound_destroy(sound->sound);
    sfSoundBuffer_destroy(sound->buffer);
    free(sound->name);
    free(sound);
}

static void destroy_firstsound(void)
{
    sound_t *next = NULL;

    if (*get_soundbank() == NULL)
        return;
    next = (*get_soundbank())->next;
    free_sound(*get_soundbank());
    *get_soundbank() = next;
}

static void destroy_lastsound(void)
{
    sound_t *list = *get_soundbank();

    if (list->next == NULL) {
        free_sound(*get_soundbank());
        return;
    }
    while (list->next->next != NULL)
        list = list->next;
    free_sound(list->next);
    list->next = NULL;
}

void destroy_sound(sound_t *sound)
{
    sound_t *list = *get_soundbank();
    sound_t *tmp = NULL;

    if (strcmp(list->name, sound->name) == 0) {
        destroy_firstsound();
        return;
    }
    while (list->next != NULL) {
        if (strcmp(list->next->name, sound->name) == 0)
            break;
        list = list->next;
    }
    if (list->next == NULL) {
        destroy_lastsound();
        return;
    }
    tmp = list->next;
    list->next = tmp->next;
    free_sound(tmp);
}
