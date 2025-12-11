//Bendeguz Huszagh and Alavi Al Alindo
#include "header.hpp"

int main(int argc, char* argv[]){
    al_init();
        srand(time(0));

    //create display, check if successful
    ALLEGRO_DISPLAY* display;
    display = al_create_display(SCREEN_W,SCREEN_H);
    if(!display){
        printf("display could not be created");
        return 1;
    }

    //initialize addons, check for success
    if (!(al_init_primitives_addon() &&
    al_install_keyboard() &&
    al_install_mouse() &&
    al_init_image_addon() &&
    al_init_font_addon() &&
    al_init_ttf_addon())){
        printf("Failed initialization");
        return 1;
    };

    //choose font for timer, check for success
    ALLEGRO_FONT* timerCountFont = al_load_font("assets/fonts/Anta-Regular.ttf", 50, 0);

    if(!timerCountFont){
        printf("failed to load font");
        return 1;
    }

    //initialize timers and event queue
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_TIMER* blizzard_timer = al_create_timer(BLIZZARD_DELAY);
    ALLEGRO_TIMER* secondTick = al_create_timer(1);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(blizzard_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(secondTick));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    ALLEGRO_KEYBOARD_STATE keystate;

    //load map tiles
    ALLEGRO_BITMAP* snowTiles[6] = {al_load_bitmap("assets/images/snowTiles/Snow_0.png"),
                                    al_load_bitmap("assets/images/snowTiles/Snow_1.png"),
                                    al_load_bitmap("assets/images/snowTiles/Snow_2.png"),
                                    al_load_bitmap("assets/images/snowTiles/Snow_3.png"),
                                    al_load_bitmap("assets/images/snowTiles/Snow_4.png"),
                                    al_load_bitmap("assets/images/snowTiles/Snow_5.png"),
                                    };

    for(int i = 0; i < 6; i++){
        if(!snowTiles[i]){
            printf("tiles initialization failed");
        }
    }

    //calculate number of tiles needed
    int numCols = MAP_WIDTH / al_get_bitmap_width(snowTiles[0]) + 5;
    int numRows = MAP_HEIGHT / al_get_bitmap_height(snowTiles[0]) + 5;
    int totalTiles = numCols * numRows;

    //initialize timer
    int TimeRemaining = 100;
    char TimerString[15];

    float cameraPosition[2] = {0,0};

    ALLEGRO_TRANSFORM camera;
    float fadeRate = 0.0;

    int* tileIndexes = (int*)malloc(sizeof(int)*totalTiles);
    resetTerrain(tileIndexes,totalTiles,sizeof(snowTiles)/sizeof(snowTiles[0]));
   
    int currentCloudCount = 0;
    int cloudFrameCount = 0;
    Cloud* cloudList = (Cloud*)malloc(sizeof(Cloud)*MAX_CLOUD_COUNT);

    ALLEGRO_BITMAP* playerDown = al_load_bitmap("assets/images/Assassin/Spritesheets/Front - Walking.png");
    ALLEGRO_BITMAP* playerUp = al_load_bitmap("assets/images/Assassin/Spritesheets/Back - Walking.png");
    ALLEGRO_BITMAP* playerLeft = al_load_bitmap("assets/images/Assassin/Spritesheets/Left - Walking.png");
    ALLEGRO_BITMAP* playerRight = al_load_bitmap("assets/images/Assassin/Spritesheets/Right - Walking.png");
    ALLEGRO_BITMAP* player;

    if(!(playerDown && playerUp && playerLeft && playerRight)){
        printf("player images initialization failded");
        return 1;
    }

    bool run = true;
    bool draw = true;


    bool active = false;
    int aniX = 0;
    int aniY = 0;
    float frameDelay = 0;
    int delayTerrainChange = TERRAIN_FRAME_DELAY_MAX;

    if(!player){
        printf("plauer image error");
        return 1;
    }

    float x = 20;
    float y = 20;


    int px = 0;
    int qx = SCREEN_W;
    int direction = DOWN;
    ALLEGRO_COLOR ccc = al_map_rgb(213,5,200);

    int currentCollObjCount = 0;
    CollisionObject* collObjs = (CollisionObject*)malloc(sizeof(CollisionObject) * 10);

    createCollisionObject(collObjs, &currentCollObjCount, playerDown, 300, 300);
    createCollisionObject(collObjs, &currentCollObjCount, playerLeft, 2800, 400);

    // al_hide_mouse_cursor(display);
    player = playerUp;
    
    double last_time = al_get_time();

    int secondTickDelay = 0;

    al_start_timer(blizzard_timer);
    al_start_timer(secondTick);
    al_start_timer(timer);
    while (run){
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue,&events);

        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            run = false;
            break;
        }

        // else if(events.type == ALLEGRO_EVENT_MOUSE_AXES){
        //     x = events.mouse.x;
        //     y = events.mouse.y;
        // }
        // else if(events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
        //     if(events.mouse.button & 1){
        //         ccc = color1;
        //     }
        //     if(events.mouse.button & 2){
        //         ccc = color2;
        //     }
        // }


        else if(events.type == ALLEGRO_EVENT_TIMER){
           if (events.timer.source == timer){
                active = true;
            delayTerrainChange++;
            cloudFrameCount++;
            secondTickDelay++;


            al_get_keyboard_state(&keystate);
            if(al_key_down(&keystate,ALLEGRO_KEY_UP)){
                y -= SPEED;
                direction = UP;
                player = playerUp;
            }
            else if(al_key_down(&keystate,ALLEGRO_KEY_DOWN)){
                y += SPEED;
                direction = DOWN;
                player = playerDown;
            }
            else if(al_key_down(&keystate,ALLEGRO_KEY_LEFT)){
                x -= SPEED;
                direction = LEFT;
                player = playerLeft;
            }
            else if(al_key_down(&keystate,ALLEGRO_KEY_RIGHT)){
                x += SPEED;
                direction = RIGHT;
                player = playerRight;
            }
            else{
                active = false;
                aniX = 0;
                aniY = 0;
            }


            if(active){
                frameDelay += 1;
                if(frameDelay >= FRAME_DELAY_MAX){
                    aniX = (aniX + 1);
                    if(aniX >= 4){
                        aniY = (aniY+1) % 5;
                        aniX = 0;
                    }
                    frameDelay = 0;
                }
            }

            if (secondTickDelay > FPS) {
                TimeRemaining--;
                secondTickDelay = 0;
            }
            cameraUpdate(cameraPosition,x,y,pSpriteX,pSpriteY);
            al_identity_transform(&camera);
            al_translate_transform(&camera,-cameraPosition[0],-cameraPosition[1]);
            al_scale_transform(&camera,SCALE_SCREEN,SCALE_SCREEN);
            al_use_transform(&camera);

            for(int i = 0; i < currentCollObjCount; i++){
                if(collisionCheck(x,y,collObjs[i].PosX,collObjs[i].PosY,pSpriteX,pSpriteY,pSpriteX,pSpriteY)){
                    if(direction == UP){
                        y += SPEED;
                    }
                    else if(direction == DOWN){
                        y -= SPEED;
                    }
                    else if(direction == LEFT){
                        x += SPEED;
                    }
                    else if(direction == RIGHT){
                        x -= SPEED;
                    }
                }
            }
        


            if(cloudFrameCount >= CLOUD_DELAY_FRAME){
                if(currentCloudCount <= MAX_CLOUD_COUNT){
                    createCloud(cloudList,&currentCloudCount,cameraPosition[0]);
                }
                cloudFrameCount = 0;
            }


            // if(delayTerrainChange >= TERRAIN_FRAME_DELAY_MAX){
            //     resetTerrain(tileIndexes,totalTiles,sizeof(snowTiles)/sizeof(snowTiles[0]));
            //     delayTerrainChange = 0;
            // }


            if(px < -SCREEN_W){
                px = SCREEN_W;
            }
            if(qx < -SCREEN_W){
                qx = SCREEN_W;
            }
            px -= 10;
            qx -= 10;


            draw = true;
           }

        //    if (events.timer.source == blizzard_timer) {
        //         double now = al_get_time();
        //         double dura = now - last_time;


        //         printf("Blizzard!!!!!\n");
        //         printf("took: %lf secs\n", dura);
        //     }
            
        }


        if(draw){
            draw = false;
            al_clear_to_color(al_map_rgb(0,0,0));

            int i = 0;
            for (int r = 0; r < numRows; r++) {
                for (int c = 0; c < numCols; c++) {
                        al_draw_bitmap(
                            snowTiles[2], c * 32, r * 32, 0
                        );
                    i++;
                }
            }

           
            // al_draw_filled_rectangle(px,0,px+SCREEN_W,SCREEN_H,al_map_rgb(2,50,120));
            // al_draw_filled_rectangle(qx,0,qx+SCREEN_W,SCREEN_H,al_map_rgb(120,50,2));
            // al_draw_filled_rectangle(x,y,x+100,y+100,c);
            al_draw_bitmap_region(player,aniX*pSpriteX,aniY*pSpriteY,pSpriteX,pSpriteY,x,y,0);

            for(int i = 0; i < currentCollObjCount; i++){
                CollisionObject cObj = collObjs[i];
                al_draw_bitmap_region(cObj.image,0,0,pSpriteX,pSpriteY,cObj.PosX,cObj.PosY,0);
            }
            for(int i = 0; i < currentCloudCount; i++){
                Cloud cCloud = cloudList[i];
                al_draw_filled_ellipse(cCloud.posx,cCloud.posy,cCloud.cloud_width,cCloud.cloud_height,al_map_rgba(15, 15, 15, 70));
                cloudList[i].posx -= cloudList[i].speed;
            }
            
            UpdateTimerString(TimerString, TimeRemaining);
            al_draw_text(timerCountFont,al_map_rgb(34,35,125),SCREEN_W/2 + cameraPosition[0], 100 + cameraPosition[1],ALLEGRO_ALIGN_CENTRE,TimerString);

            destroyCloud(cloudList,&currentCloudCount,cameraPosition[0]);

            al_flip_display();
        }
    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_bitmap(player);
    al_destroy_bitmap(playerDown);
    al_destroy_bitmap(playerUp);
    al_destroy_bitmap(playerLeft);
    al_destroy_bitmap(playerRight);
    al_destroy_font(timerCountFont);
    for (int i = 0; i < 6; i++) {
    if (snowTiles[i]) {
        al_destroy_bitmap(snowTiles[i]);
        }
    }
    free(tileIndexes);
    free(cloudList);
    free(collObjs);

    al_destroy_event_queue(event_queue);
   


    return 0;
}


void resetTerrain(int* tileIndexList, int requiredTiles, int numOfTiles){

    for (int i = 0; i < requiredTiles; i++) {
    tileIndexList[i] = 2; // Example
}

}


void createCloud(Cloud* cloudList, int* ind, int camX){


    int w = rand() % (150 + 1 - 100) + 100;
    int h = rand() % (100 + 1 - 60) + 60;
    int y = rand() % (SCREEN_H-100 + 1 - 0) + 0;
    int x = SCREEN_W + camX + rand() % (SCREEN_W  + 1 - SCREEN_W / 5) + SCREEN_W / 5;
    int s = rand() % (5 + 1 - 3) + 3;


    Cloud newCloud;
    newCloud.cloud_width = w;
    newCloud.cloud_height = h;
    newCloud.posx = x;
    newCloud.posy = y;
    newCloud.speed = s;


    cloudList[*ind] = newCloud;
    (*ind)++;
}

void destroyCloud(Cloud* cloudList, int* ind, int camX)
{
    int place = 0;
    int leftBound = camX - 200;  // allow some buffer

    for (int i = 0; i < *ind; i++) {
        Cloud cCloud = cloudList[i];

        if (cCloud.posx > leftBound) {
            cloudList[place] = cCloud;
            place++;
        }
    }

    *ind = place;
}

void createCollisionObject(CollisionObject* objList, int* currentIndex, ALLEGRO_BITMAP* image, int posX, int posY){
    CollisionObject newObj;
    newObj.image = image;
    newObj.PosX = posX;
    newObj.PosY = posY;

    objList[*currentIndex] = newObj;
    (*currentIndex)++;

}


bool collisionCheck(int px, int py, int ox, int oy, int pwidth, int pheight, int owidth, int oheight){
    if(px > ox + owidth || px + pwidth < ox || py > oy + oheight || py + pheight < oy){
        return false;
    }
    else{
        return true;
    }
}



bool collisionCheckRadius(int px, int py, int ox, int oy, int pradius, int oradius){
    if(sqrt(pow(px - ox, 2) + pow(py - oy, 2)) < pradius + oradius){
        return true;
    }
    else{
        return false;
    }
}

void cameraUpdate(float* cameraPosition, int px, int py, int pWidth, int pHeight)
{
    int playerCenterX = px + pWidth / 2;
    int playerCenterY = py + pHeight / 2;

    // Horizontal deadzone
    if (playerCenterX < cameraPosition[0] + DEADZONE_LEFT)
        cameraPosition[0] = playerCenterX - DEADZONE_LEFT;

    else if (playerCenterX > cameraPosition[0] + DEADZONE_RIGHT)
        cameraPosition[0] = playerCenterX - DEADZONE_RIGHT;

    // Vertical deadzone
    if (playerCenterY < cameraPosition[1] + DEADZONE_TOP)
        cameraPosition[1] = playerCenterY - DEADZONE_TOP;

    else if (playerCenterY > cameraPosition[1] + DEADZONE_BOTTOM)
        cameraPosition[1] = playerCenterY - DEADZONE_BOTTOM;
}

void UpdateTimerString(char* timerString, int timeRemaining){
    snprintf(timerString,15,"ddd%d",timeRemaining);
}