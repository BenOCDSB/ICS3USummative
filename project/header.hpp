#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>

#define PINK  al_map_rgb(255, 0, 255)
#define KEY_SEEN     1
#define KEY_DOWN     2

const int WIDTH = 1080;
const int HEIGHT = 720;
const int SPEED = 5;

