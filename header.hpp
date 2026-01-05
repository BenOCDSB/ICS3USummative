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
const int DEADZONE_FACTOR = 300 * 2;
float SCALE_SCREEN = 2;
#else
const int SCREEN_W = 1080;
const int SCREEN_H = 720;
const int DEADZONE_FACTOR = 300;
float SCALE_SCREEN = 1;
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
struct MessageBox{
    char msg[150] = "";
};
struct InteracObject{
    int interact = 1;
    int hp = NULL;
    int phase = NULL;
    int maxUsage = NULL;
    int usageRemain = NULL;
    MessageBox showmsg;
};
struct CollisionObject{
    int Type;
    int ID = NULL;
    ALLEGRO_BITMAP* image;
    int PosX;
    int PosY;
    int width;
    int height;
    int Padx = 0;
    int Padyt = 0;
    int Padyb = 0;

    InteracObject interaction;
};
struct BlizzardEffect{
    ALLEGRO_BITMAP* image;
    bool active = false;
    int speed = 4;
    int x;
    int y;
    int count = 0;
    int delay = 60 * 60;
};

#define Tile 0
#define Tree 1
#define Log 2
#define Rock 3
#define Pile 4
#define CutTree 5
#define Matchbox 6
#define Gasoline 7
#define Chainsaw 8
#define PlayerCord 9
#define Free 10

char GREETING[] = "Hello";
char GREETING2[] = "Player";
char BLIZZARDWARNING[] = "BLIZZARD INCOMING";
char WINTEXT[] = "YOU ESCAPED";
char LOSETEXT[] = "YOU DIED";
char STARTTEXT[] = "Press 'SPACE' to start";

const int SPEED = 6;
const int FPS = 60;
const int GAMETIME = 300;

int MAP_WIDTH = 1080 * 5;
int MAP_HEIGHT = 720 * 5;

const int DEADZONE_LEFT  = DEADZONE_FACTOR / SCALE_SCREEN;
const int DEADZONE_RIGHT = (SCREEN_W - DEADZONE_FACTOR) / SCALE_SCREEN;
const int DEADZONE_TOP = DEADZONE_FACTOR / SCALE_SCREEN;
const int DEADZONE_BOTTOM = (SCREEN_H - DEADZONE_FACTOR) / SCALE_SCREEN;

enum {UP,DOWN,LEFT,RIGHT};

const int FRAME_DELAY_MAX = 7;
const int TERRAIN_FRAME_DELAY_MAX = 5;

const int CLOUD_DELAY_FRAME = 10;
const int MAX_CLOUD_COUNT = 12;
const int CLOUD_SPEED = 2;

const int BLIZZARD_DELAY = 5;

const int interacRect = 25;

void loadMap(int* tileList, int tileSize, int rows, int cols, int* colInd, CollisionObject* colObjList, ALLEGRO_BITMAP* treeImg[3], int* plx, int* ply);
void sortColObjs(CollisionObject* objs, int listLen);

void registerRecord(int data);

void createCloud(Cloud* cloudList, int* ind, int camX);
void destroyCloud(Cloud* cloudList, int* ind, int camX);

void createCollisionObject(CollisionObject* objList, int* currentIndex, CollisionObject obj, InteracObject interac);
void deleteCollisionObject(CollisionObject* objList, int* currentIndex, int ID);

void updateBlizzard(BlizzardEffect* blizzard);

bool collisionCheck(int px, int py, int ox, int oy, int pwidth, int pheight, int owidth, int oheight, int opadx, int opadyt, int opadyb);
bool collisionCheckRadius(int px, int py, int ox, int oy, int pradius, int oradius);
void cameraUpdate(float* cameraPosition, int px, int py, int pWidth, int pHeight);

void UpdateTimerString(char* timerString, int timeRemaining);
void createMessageBox(MessageBox* currentMessages, int* currentIndex, char* message);
void deleteMessageBox(MessageBox* currentMessages, int* currentIndex);
