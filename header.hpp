#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#ifdef __APPLE__
const int SCREEN_W = 1080 * 2;
const int SCREEN_H = 720 * 2;
#else
const int SCREEN_W = 1080;
const int SCREEN_H = 720;
#endif

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

struct Cloud{
    int cloud_width;
    int cloud_height;
    int posx;
    int posy;
    int speed;
};

struct CollisionObject{
    ALLEGRO_BITMAP* image;
    int PosX;
    int PosY;
};

const int SPEED = 10;
const int FPS = 60;

float SCALE_SCREEN = 1;

int MAP_WIDTH = SCREEN_W * 3;
int MAP_HEIGHT = SCREEN_H * 3;

const int DEADZONE_FACTOR = 200;

const int DEADZONE_LEFT  = DEADZONE_FACTOR / SCALE_SCREEN;
const int DEADZONE_RIGHT = (SCREEN_W - DEADZONE_FACTOR)  / SCALE_SCREEN;
const int DEADZONE_TOP = DEADZONE_FACTOR / SCALE_SCREEN;
const int DEADZONE_BOTTOM = (SCREEN_H - DEADZONE_FACTOR) / SCALE_SCREEN;

enum {UP,DOWN,LEFT,RIGHT};

#define color1 al_map_rgb(221,200,7)
#define color2 al_map_rgb(150,200,10)

const int FRAME_DELAY_MAX = 1;
const int TERRAIN_FRAME_DELAY_MAX = 5;

const int CLOUD_DELAY_FRAME = 10;
const int MAX_CLOUD_COUNT = 4;
const int CLOUD_SPEED = 2;

const int BLIZZARD_DELAY = 5;

const float pSpriteX = 240;
const float pSpriteY = 240;

void resetTerrain(int* tileIndexList, int requiredTiles, int numOfTiles);
void createCloud(Cloud* cloudList, int* ind, int camX);
void destroyCloud(Cloud* cloudList, int* ind, int camX);

void createCollisionObject(CollisionObject* objList, int* currentIndex, ALLEGRO_BITMAP* image, int posX, int posY);


bool collisionCheck(int px, int py, int ox, int oy, int pwidth, int pheight, int owidth, int oheight);
bool collisionCheckRadius(int px, int py, int ox, int oy, int pradius, int oradius);
void cameraUpdate(float* cameraPosition, int px, int py, int pWidth, int pHeight);

void UpdateTimerString(char* timerString, int timeRemaining);
