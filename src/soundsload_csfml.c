/*
** EPITECH PROJECT, 2024
** soundsload_csfml
** File description:
** Sound loading (precaching) related functions.
*/

#include "../include/header_csfml.h"

sound_t *make_sound(char *name, char const *file)
{
    sound_t *sound = malloc(sizeof(sound_t));

    if (sound == NULL)
        return NULL;
    sound->name = strdup(name);
    sound->sound = sfSound_create();
    sound->buffer = sfSoundBuffer_createFromFile(file);
    sfSound_setBuffer(sound->sound, sound->buffer);
    sound->volume = 0.0;
    sound->pitch = 1.0;
    sound->next = *get_soundbank();
    *get_soundbank() = sound;
    return sound;
}
