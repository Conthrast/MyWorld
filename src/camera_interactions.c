/*
** EPITECH PROJECT, 2024
** camera_interactions
** File description:
** Camera interaction related functions.
*/

#include "../include/header.h"

static void cam_mouse_limits(cam_t *cam)
{
    if (cam->rotation > 360.0)
        cam->rotation -= 360.0;
    else if (cam->rotation < 0)
        cam->rotation += 360.0;
    if (cam->tilt > 0.8)
        cam->tilt = 0.8;
    else if (cam->tilt < 0)
        cam->tilt = 0;
}

static void cam_mouse_axis(sfMouseMoveEvent mouse,
    sfMouseMoveEvent mouseold, cam_t *cam)
{
    if (get_tween("camrot") != NULL)
        destroy_tween("camrot");
    if (get_tween("camtilt") != NULL)
        destroy_tween("camtilt");
    cam->rotation += (mouseold.x - mouse.x) / 5.0;
    cam->tilt -= (mouseold.y - mouse.y) / 200.0;
}

static void cam_mouse_offset(sfMouseMoveEvent mouse,
    sfMouseMoveEvent mouseold, cam_t *cam)
{
    if (get_tween("camcentx") != NULL && get_tween("camcenty") != NULL) {
        destroy_tween("camcentx");
        destroy_tween("camcenty");
    }
    cam->offset.x -= (mouseold.x - mouse.x) / cam->zoom;
    cam->offset.y -= (mouseold.y - mouse.y) / cam->zoom;
}

static void cam_mouse_move(sfMouseMoveEvent mouse, cam_t *cam)
{
    static sfMouseMoveEvent mouseold;

    if (GAMESTUFF->curs_state != CAMERA && !GAMESTUFF->fullscreen
    && !(mouse.x > 250 && mouse.y > 30 && mouse.y < 440)) {
        mouseold = mouse;
        return;
    }
    GAMESTUFF->curs_state = CAMERA;
    if (MOUSEPRESS(sfMouseLeft))
        cam_mouse_axis(mouse, mouseold, cam);
    else if (MOUSEPRESS(sfMouseRight))
        cam_mouse_offset(mouse, mouseold, cam);
    else
        GAMESTUFF->curs_state = NOSTATE;
    cam_mouse_limits(cam);
    mouseold = mouse;
}

void cam_zoom(sfMouseWheelScrollEvent mouse, cam_t *cam)
{
    tween_t *tween = get_tween("camzoom");
    float tmp = cam->zoom;

    if (tween != NULL)
        tmp = tween->dest;
    tween = make_tween("camzoom", &cam->zoom, tmp, 0.5);
    tween->method = EASEOUT;
    tmp += mouse.delta * tmp / 10;
    if (tmp > 500)
        tmp = 500;
    else if (tmp < 4 + GAMESTUFF->fullscreen * 1.6)
        tmp = 4 + GAMESTUFF->fullscreen * 1.6;
    tween->dest = tmp;
}

void cam_rotate(cam_t *cam)
{
    tween_t *tween = get_tween("camrot");
    int tmp = cam->rotation;

    if (tween != NULL)
        tmp = tween->dest;
    tween = make_tween("camrot", &cam->rotation, tmp, 1.7);
    tween->method = EASEOUT;
    if (KEYPRESS(sfKeyLeft))
        tmp += 90;
    else if (KEYPRESS(sfKeyRight))
        tmp -= 90;
    if (tmp > 360.0) {
        tmp -= 360.0;
        tween->start -= 360.0;
    } else if (tmp < 0) {
        tmp += 360.0;
        tween->start += 360.0;
    }
    tween->dest = (tmp - 22) / 90 * 90 + 45;
    play_sound("camspin", 75, 1.0);
}

static void cam_tilt_sound(int up)
{
    if (up)
        play_sound("camup", 75, 1.0);
    else
        play_sound("camdown", 75, 1.0);
}

void cam_tilt(cam_t *cam)
{
    tween_t *tween = get_tween("camtilt");
    float tmp = cam->tilt;

    if (tween != NULL)
        tmp = tween->dest;
    tween = make_tween("camtilt", &cam->tilt, tmp, 0.75);
    tween->method = EASEOUT;
    if (KEYPRESS(sfKeyUp))
        tmp -= 0.1;
    else if (KEYPRESS(sfKeyDown))
        tmp += 0.1;
    if (tmp > 0.85) {
        tmp = 0.8;
        play_sound("cambuzz", 75, 1.0);
    } else if (tmp < -0.05) {
        tmp = 0;
        play_sound("cambuzz", 75, 1.0);
    } else
        cam_tilt_sound(KEYPRESS(sfKeyUp));
    tween->dest = tmp;
}

void cam_reset(cam_t *cam)
{
    run_timer("camres", 0.5);
    make_tween("camrot", &cam->rotation, 45, 1.5)->method = EASEOUT;
    make_tween("camtilt", &cam->tilt, 0.5, 1.5)->method = EASEOUT;
    make_tween("camzoom", &cam->zoom, cam->zoomdef, 2)->method = EASEOUT;
    make_tween("camcentx", &cam->offset.x, 0, 1.5)->method = EASEOUT;
    make_tween("camcenty", &cam->offset.y, 0, 1.5)->method = EASEOUT;
}

void cam_interaction(sfEvent event, cam_t *cam)
{
    if (event.type == sfEvtMouseMoved)
        cam_mouse_move(event.mouseMove, cam);
    if (event.type == sfEvtKeyPressed) {
        if (get_timer("camrot") == NULL
        && (KEYPRESS(sfKeyLeft) || KEYPRESS(sfKeyRight))) {
            run_timer("camrot", 0.2);
            cam_rotate(cam);
        }
        if (get_timer("camtilt") == NULL
        && (KEYPRESS(sfKeyUp) || KEYPRESS(sfKeyDown))) {
            run_timer("camtilt", 0.2);
            cam_tilt(cam);
        }
        if (get_timer("camres") == NULL && KEYPRESS(sfKeyR)) {
            play_sound("reset", 40, diceroll(90, 110) / 100.0);
            cam_reset(cam);
        }
    }
    if (event.type == sfEvtMouseWheelScrolled)
        cam_zoom(event.mouseWheelScroll, cam);
}
