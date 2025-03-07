/*
** EPITECH PROJECT, 2024
** main
** File description:
** Main file for the MyWorld program.
*/

#include "../include/header.h"

void readme_print(void)
{
    FILE *file = fopen("README.md", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    if (file == NULL)
        return;
    while (read != -1) {
        if (read != 0)
            write(1, line, read);
        read = getline(&line, &len, file);
    }
    free(line);
    fclose(file);
}

void events(world_t **world, sfVertex ***vertex, cam_t **cam)
{
    sfEvent event;

    while (sfRenderWindow_pollEvent(WINDOW, &event)) {
        cam_interaction(event, *cam);
        vertex_interaction(event, *world, *vertex, *cam);
        if (event.type == sfEvtKeyPressed)
            game_hotkeys(event.key.code, world, vertex, cam);
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(WINDOW);
    }
}

void run(void)
{
    world_t *world = init_world(diceroll(3, 64));
    sfVertex **vertex = init_vertexes(world);
    sfVertexArray *tile = sfVertexArray_create();
    cam_t *cam = init_cam(world->size);

    sfVertexArray_setPrimitiveType(tile, sfQuads);
    while (sfRenderWindow_isOpen(WINDOW)) {
        update_technicalstuff(world);
        update_background(cam);
        update_render(world, vertex, tile, cam);
        update_hud(world, vertex, tile);
        update_vertexes(world, vertex, cam);
        events(&world, &vertex, &cam);
        sfRenderWindow_display(WINDOW);
    }
    sfVertexArray_destroy(tile);
    save_world_prompt(world);
    destroy_world(world, vertex);
    destroy_assets(cam);
}

int main(int ac, char **av)
{
    if (ac > 1 && strcmp(av[1], "-h") == 0) {
        readme_print();
        return 0;
    }
    srand(time(NULL));
    init_assets();
    init_gamestuff();
    init_renderstate();
    text_jumpscare("READ THE README PLEASE!!!", 5);
    run();
    return 0;
}
