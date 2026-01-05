#include "header.hpp"

ALLEGRO_BITMAP* logImg;
ALLEGRO_BITMAP* rockImg;
ALLEGRO_BITMAP* snowPileImg;

ALLEGRO_BITMAP* matchBoxImg;
ALLEGRO_BITMAP* gasolineImg;
ALLEGRO_BITMAP* chainsawImg;
ALLEGRO_BITMAP* freeImg;

int main(int argc, char* argv[]){
    al_init();
        srand(time(0));


    ALLEGRO_DISPLAY* display;
    display = al_create_display(SCREEN_W,SCREEN_H);

    if(!display){
        printf("display could not be created");
        return 1;
    }

    // // same size on all device...
    // int fb_w = al_get_display_width(display);
    // int fb_h = al_get_display_height(display);

    // float sx = (float)fb_w / SCREEN_W;
    // float sy = (float)fb_h / SCREEN_H;

    // ALLEGRO_TRANSFORM t;z
    // al_identity_transform(&t);
    // al_scale_transform(&t, sx, sy);
    // al_use_transform(&t);
    // // ...


    if (!(al_init_primitives_addon() &&
    al_install_keyboard() &&
    al_install_mouse() &&
    al_init_image_addon() &&
    al_init_font_addon() &&
    al_init_ttf_addon())){
        printf("Failed initialization");
        return 1;
    };


    ALLEGRO_FONT* timerCountFont = al_load_font("assets/fonts/Iceberg-Regular.ttf", 50, 0);
    ALLEGRO_FONT* WarningFont = al_load_font("assets/fonts/monogram-extended.ttf", 70, 0);
    ALLEGRO_FONT* overFont = al_load_font("assets/fonts/ThaleahFat.ttf", 100, 0);
    ALLEGRO_FONT* startFont = al_load_font("assets/fonts/monogram-extended.ttf", 100, 0);
    ALLEGRO_FONT* MessageFont = al_load_font("assets/fonts/monogram-extended.ttf", 35, 0);
    ALLEGRO_FONT* MessageFont2 = al_load_font("assets/fonts/ThaleahFat.ttf", 20, 0);
    
    if(!timerCountFont){
        printf("failed to load font");
        return 1;
    }


    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_TIMER* secondTick = al_create_timer(1);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue,al_get_timer_event_source(secondTick));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    ALLEGRO_KEYBOARD_STATE keystate;


    ALLEGRO_BITMAP* snowTiles[3] = {al_load_bitmap("assets/images/Snow tile files/Snow tile.png"),
                                    al_load_bitmap("assets/images/Snow tile files/Snow tile with greenery.png"),
                                    al_load_bitmap("assets/images/Snow tile files/Cliff_Right_1.png"),
                                   };

    for(int i = 0; i < 2; i++){
        if(!snowTiles[i]){
            printf("tiles initialization failed");
        }
    }
    ALLEGRO_BITMAP* treeImgs[3] = {al_load_bitmap("assets/images/Evergreen tree files/Burning evergreen tree.png"),
                                    al_load_bitmap("assets/images/Evergreen tree files/Snowy evergreen tree.png"),
                                    al_load_bitmap("assets/images/Evergreen tree files/Evergreen tree.png"),
                                   };

    for(int i = 0; i < 3; i++){
        if(!treeImgs[i]){
            printf("tree initialization failed");
        }
    }

    logImg = al_load_bitmap("assets/images/Snowy log file/Snowy log.png");
    rockImg = al_load_bitmap("assets/images/Rock file/Rock.png");
    snowPileImg = al_load_bitmap("assets/images/Snow pile file/Snow pile.png");

    matchBoxImg = al_load_bitmap("assets/images/Gasoline can file/matchbox.png");
    gasolineImg = al_load_bitmap("assets/images/Gasoline can file/Gasoline can.png");
    chainsawImg = al_load_bitmap("assets/images/Chainsaw file/Chainsaw.png");

    freeImg = al_load_bitmap("assets/images/Snow tile files/goldentile.png");

    if(!(logImg && rockImg && snowPileImg && matchBoxImg && gasolineImg && chainsawImg && freeImg)){
        printf("asset load failed 1");
    }


    int numCols = MAP_WIDTH / al_get_bitmap_width(snowTiles[0]);
    int numRows = MAP_HEIGHT / al_get_bitmap_height(snowTiles[0]) + 4;
    int totalTiles = numCols * numRows;
    int tileSize = al_get_bitmap_width(snowTiles[0]);
    int* tileIndexes = (int*)malloc(sizeof(int)*totalTiles);

    int currentCollObjCount = 0;
    CollisionObject* collObjs = (CollisionObject*)malloc(sizeof(CollisionObject) * 1000);


    int TimeRemaining = GAMETIME;
    char TimerString[15];

    ALLEGRO_TRANSFORM camera;
    float fadeRate = 15.0f;

    
    int currentCloudCount = 0;
    int cloudFrameCount = 0;
    Cloud* cloudList = (Cloud*)malloc(sizeof(Cloud)*MAX_CLOUD_COUNT+5);
    ALLEGRO_BITMAP* cloudImg = al_load_bitmap("assets/images/Cloud shadow file/Cloud shadow.png");

    ALLEGRO_BITMAP* inventoryImg = al_load_bitmap("assets/images/other/transparentInventory.png");
    ALLEGRO_BITMAP* mbImg = al_load_bitmap("assets/images/other/mbtran.png");
    ALLEGRO_BITMAP* startImg = SCALE_SCREEN == 1 ? al_load_bitmap("assets/images/other/startImg.jpg") : al_load_bitmap("assets/images/other/startImg2.jpg");

    ALLEGRO_BITMAP* blizzardImg = al_load_bitmap("assets/images/Blizzard effect/Untitled (2000 x 1160 px).png");
    BlizzardEffect blizzard;
    blizzard.image = blizzardImg;

    if(!cloudImg || !inventoryImg || !mbImg || !blizzardImg){
        printf("asset error\n");
        return 1;
    }

    int currentMessageCount = 0;
    MessageBox activeMessages[5];
    createMessageBox(activeMessages,&currentMessageCount,GREETING);
    createMessageBox(activeMessages,&currentMessageCount,GREETING2);

    ALLEGRO_BITMAP* playerDown[3] = {al_load_bitmap("assets/images/Character files/down-1.png"),
                                    al_load_bitmap("assets/images/Character files/down-2.png"),
                                    al_load_bitmap("assets/images/Character files/down-3.png")};

    ALLEGRO_BITMAP* playerUp[3] = {al_load_bitmap("assets/images/Character files/up-1.png"),
                                    al_load_bitmap("assets/images/Character files/up-2.png"),
                                    al_load_bitmap("assets/images/Character files/up-3.png")};

    ALLEGRO_BITMAP* playerLeft[3] = {al_load_bitmap("assets/images/Character files/left-1.png"),
                                    al_load_bitmap("assets/images/Character files/left-2.png"),
                                    al_load_bitmap("assets/images/Character files/left-3.png")};

    ALLEGRO_BITMAP* playerRight[3] = {al_load_bitmap("assets/images/Character files/right-1.png"),
                                    al_load_bitmap("assets/images/Character files/right-2.png"),
                                    al_load_bitmap("assets/images/Character files/right-3.png")};

    ALLEGRO_BITMAP** player = playerDown;

    if(!(playerDown[0] && playerUp[0] && playerLeft[0] && playerRight[0])){
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


    int pAniInd = 0;
    player = playerDown;
    if(!player){
        printf("player image error");
        return 1;
    }

    int x = 20;
    int y = 20;
    int pWidth = al_get_bitmap_width(player[0]);
    int pHeight = al_get_bitmap_height(player[0]);
    int playerCX = x + pWidth / 2;
    int playerCY = y + pHeight / 2;
    CollisionObject pInventory;

    int px = 0;
    int qx = SCREEN_W;
    int direction = DOWN;
    ALLEGRO_COLOR ccc = al_map_rgb(213,5,200);

    int inx = x;
    int iny = y;
    bool epressed = false;

    // createCollisionObject(collObjs, &currentCollObjCount, playerDown, 300, 300);
    // createCollisionObject(collObjs, &currentCollObjCount, playerLeft, 2800, 400);

    bool firstRun = true;

    // Save current target (usually the display)
    // ALLEGRO_BITMAP* old_target = al_get_target_bitmap();
    // // Switch to your bitmap
    // al_set_target_bitmap(*player);
    // // Clear / fill with color
    // al_clear_to_color(al_map_rgb(255, 0, 0)); // Red fill
    // // Restore old target
    // al_set_target_bitmap(old_target);

    float cameraPosition[2] = {0, 0};
    cameraPosition[0] = -(SCREEN_W / 2.0) + (x + pWidth / 2.0);
    cameraPosition[1] = -(SCREEN_H/2.0) + (y + pHeight / 2.0);


    loadMap(tileIndexes,tileSize,numRows,numCols,&currentCollObjCount,collObjs,treeImgs,&x,&y);
    printf("Collision count after loadMap: %d\n", currentCollObjCount);

    sortColObjs(collObjs, currentCollObjCount);
    
    bool gamePaused = false;
    bool allowRun = true;
    bool GAME_OVER = true;
    bool WON = false;
    bool GameStart = false;
    int startcount = 0;

    double last_time = al_get_time();
    al_start_timer(secondTick);
    al_start_timer(timer);

    printf("ddd: %d\n", collObjs[0].Type);

    while (run){
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue,&events);
        // printf("\ncols: %d", numCols);
        // printf("\nrows: %d", numRows);

        if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            run = false;
            break;
        }

        else if(events.type == ALLEGRO_EVENT_KEY_DOWN){

                if(events.keyboard.keycode == ALLEGRO_KEY_E){

                    if(!epressed){
                        if(gamePaused){
                            deleteMessageBox(activeMessages,&currentMessageCount);
                        }
                        else{
                            inx = x;
                            iny = y + pHeight/2 - 15;

                            // iny =+ pSpriteY/2;

                            int actHeight = pHeight/2;

                            if(direction == LEFT){
                                inx -= interacRect;
                            }
                            else if(direction == RIGHT){
                                inx += pWidth;
                            }
                            else if(direction == UP){
                                inx = x + pWidth/2 - interacRect/2;
                                iny = y - interacRect;
                                actHeight += 40;
                            }
                            else if(direction == DOWN){
                                inx = x + pWidth/2 - interacRect/2;
                                iny = y + pHeight;
                            }


                            CollisionObject* cObj;
                            for(int i = 0; i < currentCollObjCount; i++){

                                CollisionObject* iterateObj = &collObjs[i];
                                
                                int margin = 200 * SCALE_SCREEN;
                                bool inVision = (
                                    iterateObj->PosX + iterateObj->width > cameraPosition[0] - margin &&
                                    iterateObj->PosX < cameraPosition[0] + SCREEN_W + margin &&
                                    iterateObj->PosY + iterateObj->height > cameraPosition[1] - margin &&
                                    iterateObj->PosY < cameraPosition[1] + SCREEN_H + margin
                                );

                                if(inVision){
                                    if(collisionCheck(inx,iny,iterateObj->PosX,iterateObj->PosY,interacRect,actHeight,iterateObj->width,iterateObj->height,0,0,0)){
                                        printf("Type: %d\n", collObjs[i].Type);
                                        cObj = iterateObj;
                                    }
                                }

                            }

                            if(cObj && cObj->interaction.interact){

                                if((*cObj).Type == Matchbox || (*cObj).Type == Gasoline || (*cObj).Type == Chainsaw){
                                    if(!pInventory.ID){
                                        pInventory = *cObj;
                                        deleteCollisionObject(collObjs,&currentCollObjCount,(*cObj).ID);
                                        printf("\niiid: %d", pInventory.ID);
                                    }
                                    else{
                                        char msg[] = "I already have something in my inventory.";
                                        createMessageBox(activeMessages, &currentMessageCount, msg);
                                    }
                                }
                                
                                
                                else if(cObj->Type == CutTree){
                                    bool cut = false;

                                    if(cObj->interaction.phase == 1){
                                        if(pInventory.Type == Gasoline){
                                            if(pInventory.interaction.usageRemain > 0){
                                                cObj->interaction.phase = 2;
                                                cObj->image = treeImgs[2];
                                                pInventory.interaction.usageRemain--;
                                                
                                                char ms1[] = "The gasoline removed the snow... totally normal...";
                                                createMessageBox(activeMessages, &currentMessageCount, ms1);
                                                strcpy(cObj->interaction.showmsg.msg, "I need to ignite it now somehow.");
                                            }
                                            else{
                                                strcpy(pInventory.interaction.showmsg.msg, "The gasoline can is empty...");
                                                createMessageBox(activeMessages, &currentMessageCount, pInventory.interaction.showmsg.msg);
                                            }

                                            cut = true;
                                        }
                                    }

                                    if(cObj->interaction.phase == 2){
                                        if(pInventory.Type == Matchbox){
                                            if(pInventory.interaction.usageRemain > 0){
                                                cObj->interaction.phase = 3;
                                                cObj->image = treeImgs[0];
                                                pInventory.interaction.usageRemain--;
                                                
                                                char ms1[] = "The tree is burning!";
                                                createMessageBox(activeMessages, &currentMessageCount, ms1);
                                                strcpy(cObj->interaction.showmsg.msg, "I need to find to something to cut it.");
                                            }
                                            else{
                                                strcpy(pInventory.interaction.showmsg.msg, "No matchstick left...");
                                                createMessageBox(activeMessages, &currentMessageCount, pInventory.interaction.showmsg.msg);
                                            }

                                            cut = true;
                                        }
                                    }

                                    if(cObj->interaction.phase == 3){
                                        if(pInventory.Type == Chainsaw){
                                            if(pInventory.interaction.usageRemain > 0){
                                                cObj->interaction.phase = 2;
                                                cObj->image = treeImgs[0];
                                                pInventory.interaction.usageRemain--;
                                                
                                                char ms1[] = "The tree is gone... vanished?";
                                                createMessageBox(activeMessages, &currentMessageCount, ms1);
                                                deleteCollisionObject(collObjs,&currentCollObjCount,cObj->ID);
                                            }
                                            else{
                                                strcpy(pInventory.interaction.showmsg.msg, "The chainsaw is out of power...");
                                                createMessageBox(activeMessages, &currentMessageCount, pInventory.interaction.showmsg.msg);
                                            }
                                        }
                                    }


                                    createMessageBox(activeMessages, &currentMessageCount, cObj->interaction.showmsg.msg);

                                }

                                else if(cObj->Type == Tree){
                                    createMessageBox(activeMessages, &currentMessageCount, cObj->interaction.showmsg.msg);
                                }

                                cObj = NULL;
                            }
                        }
                        

                        epressed = true;
                    }
                    
                }
                
                else if(events.keyboard.keycode == ALLEGRO_KEY_D){
                    if(pInventory.ID){
                        CollisionObject newObj = pInventory;
                        
                        newObj.PosX = playerCX - newObj.width/2;
                        newObj.PosY = playerCY;
                        createCollisionObject(collObjs, &currentCollObjCount, newObj, newObj.interaction);
                        pInventory = CollisionObject{};
                    }
                }

                else if(events.keyboard.keycode == ALLEGRO_KEY_SPACE){
                    if(!GameStart){
                        GAME_OVER = false;
                        GameStart = true;
                    }
                }
        }

        else if(events.type == ALLEGRO_EVENT_KEY_UP){
                if(events.keyboard.keycode == ALLEGRO_KEY_E){
                    epressed = false;
                }
            }

        else if(events.type == ALLEGRO_EVENT_TIMER){

            if(!GAME_OVER){
                if (events.timer.source == timer){
                

                draw = true;
                gamePaused = (currentMessageCount > 0);
                
                if(allowRun){
                    gamePaused = false;
                    allowRun = false;
                };

                if(!gamePaused){

                    active = true;
                    delayTerrainChange++;
                    cloudFrameCount++;

                    if(!blizzard.active){blizzard.count++;}
                    bool inBlizzard = blizzard.active && (x > blizzard.x && x < blizzard.x + al_get_bitmap_width(blizzardImg) + 10);

                    al_get_keyboard_state(&keystate);
                    if(al_key_down(&keystate,ALLEGRO_KEY_UP)){
                        y -= SPEED;
                        if(inBlizzard)y+=blizzard.speed;
                        direction = UP;
                        player = playerUp;
                    }
                    else if(al_key_down(&keystate,ALLEGRO_KEY_DOWN)){
                        y += SPEED;
                        if(inBlizzard)y-=blizzard.speed;
                        direction = DOWN;
                        player = playerDown;
                    }
                    else if(al_key_down(&keystate,ALLEGRO_KEY_LEFT)){
                        x -= SPEED;
                        if(inBlizzard)x+=blizzard.speed;
                        direction = LEFT;
                        player = playerLeft;
                    }
                    else if(al_key_down(&keystate,ALLEGRO_KEY_RIGHT)){
                        x += SPEED;
                        if(inBlizzard)x-=blizzard.speed;
                        direction = RIGHT;
                        player = playerRight;
                 }
                    else if(al_key_down(&keystate,ALLEGRO_KEY_ESCAPE)){
                        run = false;
                        break;
                    }
                    else{
                        active = false;
                        pAniInd = 0;
                        frameDelay = 0;
                    }


                    if(active){
                        frameDelay += 1;
                        if(frameDelay >= FRAME_DELAY_MAX){
                            pAniInd += 1;

                            if(pAniInd >= 3){
                                pAniInd = 0;
                            }
                            
                            frameDelay = 0;
                        }
                    }
                    playerCX = x + pWidth / 2;
                    playerCY = y + pHeight / 2;

                    cameraUpdate(cameraPosition,x,y,pWidth,pHeight);
                    al_identity_transform(&camera);
                    al_translate_transform(&camera,-cameraPosition[0],-cameraPosition[1]);
                    al_scale_transform(&camera,SCALE_SCREEN,SCALE_SCREEN);
                    al_use_transform(&camera);

                    for(int i = 0; i < currentCollObjCount; i++){
                        if(collisionCheck(x,y,collObjs[i].PosX,collObjs[i].PosY,pWidth,pHeight,collObjs[i].width,collObjs[i].height,collObjs[i].Padx,collObjs[i].Padyt,collObjs[i].Padyb)){
                            
                            if(collObjs[i].Type == Free){
                                GAME_OVER = true;
                                WON = true;
                                int finishTime = al_get_time() - last_time;
                                registerRecord(finishTime);
                            }
                            
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

                        destroyCloud(cloudList,&currentCloudCount,cameraPosition[0]);
                    }
                    
                    // update blizzard
                    if(blizzard.count >= blizzard.delay){
                        if(!blizzard.active){
                            blizzard.x = x + 500;
                            blizzard.y = 0;
                        }
                        updateBlizzard(&blizzard);
                    }
                    
            }

                if(events.timer.source == secondTick){
                if(!gamePaused){
                    
                    if(TimeRemaining <= 0){
                        GAME_OVER = true;
                        WON = false;
                    }

                    TimeRemaining--;

                    fadeRate += 240 / float(GAMETIME);
                    if(fadeRate >= 250) fadeRate = 250;

                }

           }

            }

        }

        if(!GAME_OVER){
            if(draw){
                draw = false;
                            
                al_clear_to_color(al_map_rgb(255,255,255));
                int margin = 200 * SCALE_SCREEN;

                    int i = 0;
                    for (int r = 0; r < numRows; r++) {
                        for (int c = 0; c < numCols; c++) {
                                int tx = c * al_get_bitmap_width(snowTiles[0]);
                                int ty = r * al_get_bitmap_height(snowTiles[0]);

                                bool inVision =
                                    (tx + tileSize > cameraPosition[0] - margin &&
                                    tx < cameraPosition[0] + SCREEN_W + margin &&
                                    ty + tileSize > cameraPosition[1] - margin &&
                                    ty < cameraPosition[1] + SCREEN_H + margin);


                                if(inVision){

                                    
                                    int playerBottomX = playerCX;
                                    int playerBottomY = playerCY + pHeight - tileSize;

                                    bool isBottomCenterTile =
                                        playerBottomX >= tx && playerBottomX < tx + tileSize &&
                                        playerBottomY >= ty && playerBottomY < ty + tileSize;

                                    if (isBottomCenterTile) {
                                        al_draw_bitmap(snowTiles[1], tx, ty, 0);
                                    } else {
                                        al_draw_bitmap(snowTiles[0], tx, ty, 0);
                                    }
                                    

                                }
                                
                            i++;
                        }
                    }

                    for(int i = 0; i < currentCollObjCount; i++){
                        CollisionObject* cObj = &collObjs[i];

                        if(!(cObj->Type == Tree || cObj->Type == Pile || cObj->Type == CutTree)){
                            bool inVision = (
                            cObj->PosX + cObj->width > cameraPosition[0] - margin &&
                            cObj->PosX < cameraPosition[0] + SCREEN_W + margin &&
                            cObj->PosY + cObj->height > cameraPosition[1] - margin &&
                            cObj->PosY < cameraPosition[1] + SCREEN_H + margin
                        );
                            if(inVision){
                                al_draw_bitmap(cObj->image,cObj->PosX,cObj->PosY,0);

                                // al_draw_rectangle(
                                //     cObj->PosX, cObj->PosY,
                                //     cObj->PosX + cObj->width,
                                //     cObj->PosY + cObj->height,
                                //     al_map_rgb(0,255,0),
                                //     2
                                // );
                            }
                        }
                    }
                
                    al_draw_bitmap(player[pAniInd], x, y, 0);

                    for(int i = 0; i < currentCollObjCount; i++){
                        CollisionObject cObj = collObjs[i];

                        if(cObj.Type == Tree || cObj.Type == Pile || cObj.Type == CutTree){
                            bool inVision = (
                            cObj.PosX + cObj.width > cameraPosition[0] - margin &&
                            cObj.PosX < cameraPosition[0] + SCREEN_W + margin &&
                            cObj.PosY + cObj.height > cameraPosition[1] - margin &&
                            cObj.PosY < cameraPosition[1] + SCREEN_H + margin
                        );
                            if(inVision){
                                al_draw_bitmap(cObj.image,cObj.PosX,cObj.PosY,0);
                            }
                        }
                    }

                    for(int i = 0; i < currentCloudCount; i++){
                        Cloud cCloud = cloudList[i];
                        al_draw_bitmap(cloudImg, cCloud.posx, cCloud.posy, 0);                   
                    }
                    

                    UpdateTimerString(TimerString, TimeRemaining);

                    al_draw_rectangle(inx, iny, inx+interacRect, iny+pHeight/2,
                    al_map_rgb(255,0,0), 2);

                    if(blizzard.count >= blizzard.delay - 300){
                        if(sin(0.25*blizzard.count) < 0){
                            al_draw_text(WarningFont,al_map_rgba(120, 18, 18, 200),SCREEN_W/(2*SCALE_SCREEN) + cameraPosition[0], 120 + cameraPosition[1],ALLEGRO_ALIGN_CENTRE,BLIZZARDWARNING);
                        }
                    }

                    if(blizzard.active){
                        al_draw_bitmap(blizzard.image,blizzard.x,blizzard.y+cameraPosition[1],0);
                    }

                    al_draw_bitmap(inventoryImg, SCREEN_W - 630*SCALE_SCREEN + cameraPosition[0], cameraPosition[1] + 100, 0);

                    if(pInventory.ID){
                        int ix = SCREEN_W - 630*SCALE_SCREEN + cameraPosition[0] + (al_get_bitmap_width(inventoryImg) - al_get_bitmap_width(pInventory.image)) / 2;
                        int iy = cameraPosition[1] + 100 + 10 + (al_get_bitmap_height(inventoryImg) - al_get_bitmap_height(pInventory.image)) / 2;

                        char string1[] = "Press \"D\" to drop";
                        char string2[] = "Usage: 20/20";

                        snprintf(string2, sizeof(string2), "Usage: %d/%d", pInventory.interaction.usageRemain, pInventory.interaction.maxUsage);

                        al_draw_bitmap(pInventory.image,  ix,  iy, 0);
                        al_draw_text(MessageFont2, al_map_rgb(40, 41, 40), SCREEN_W - 630*SCALE_SCREEN + cameraPosition[0], 100 + cameraPosition[1] + al_get_bitmap_height(inventoryImg) + 10, ALLEGRO_ALIGN_LEFT, string2);
                        al_draw_text(MessageFont2, al_map_rgb(40, 41, 40), SCREEN_W - 630*SCALE_SCREEN + cameraPosition[0], 100 + cameraPosition[1] + al_get_bitmap_height(inventoryImg) + 30, ALLEGRO_ALIGN_LEFT, string1);
                    }
                    

                    al_draw_filled_rectangle(0 + cameraPosition[0], 0 + cameraPosition[1],
                        SCREEN_W + cameraPosition[0], SCREEN_H + cameraPosition[1], al_map_rgba(20, 20, 20, fadeRate));

                    int tx = SCREEN_W/(2*SCALE_SCREEN) + cameraPosition[0];
                    int ty = 40 + cameraPosition[1];
                    al_draw_filled_rounded_rectangle(tx - 10 -al_get_text_width(timerCountFont, TimerString)/2, ty - 5,
                    tx + al_get_text_width(timerCountFont, TimerString)/2 + 10, ty + al_get_font_line_height(timerCountFont) + 5, 5, 5, al_map_rgba(57, 60, 64, 100));

                    al_draw_text(timerCountFont,al_map_rgb(44, 96, 163),tx,ty,ALLEGRO_ALIGN_CENTRE,TimerString);

                    if(gamePaused){
                        int y1 = (SCREEN_H/SCALE_SCREEN - 250) + cameraPosition[1];
                        int y2 = y1 + 150;
                        int x1 = (SCREEN_W - al_get_bitmap_width(mbImg)*SCALE_SCREEN)/(2*SCALE_SCREEN);
                        int x2 = x1 + 30;

                        al_draw_bitmap(mbImg, x1 + cameraPosition[0], 500 + cameraPosition[1], 0);
                        al_draw_text(MessageFont,al_map_rgb(255,255,255),x2 + cameraPosition[0],(y1+y2)/2 - 25,ALLEGRO_ALIGN_LEFT,activeMessages[0].msg);

                    }
                    
            

                al_flip_display();
            }
            
        }

        else{
            if(GameStart){
                al_clear_to_color(al_map_rgb(5,56,100));

                int tx = SCREEN_W/(2*SCALE_SCREEN) + cameraPosition[0];
                int ty = 40 + cameraPosition[1];

                ALLEGRO_COLOR color = WON ? al_map_rgb(35, 166, 83) : al_map_rgb(166, 35, 35);
                char* text = WON ? WINTEXT : LOSETEXT;

                al_draw_filled_rectangle(0 + cameraPosition[0], 0 + cameraPosition[1], cameraPosition[0] + SCREEN_W, cameraPosition[1] + SCREEN_H, al_map_rgb(20, 20, 20));
                al_draw_text(overFont, color, 
                tx, ty, ALLEGRO_ALIGN_CENTER, text);
            }
            else{
                al_clear_to_color(al_map_rgb(5,56,100));
                

                int tx = SCREEN_W/(2);
                int ty = SCREEN_H - 140;

                al_draw_bitmap(startImg, 0, 0, 0);

                if(sin(0.15 * startcount) > 0){
                    al_draw_text(startFont, al_map_rgb(255,255,255), tx, ty, ALLEGRO_ALIGN_CENTER, STARTTEXT);
                }

                startcount++;
            }

            
            al_flip_display();
        }
        
    }

    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_bitmap(*player);

    for(int i = 0; i < 3; i++){
        al_destroy_bitmap(playerDown[i]);
        al_destroy_bitmap(playerUp[i]);
        al_destroy_bitmap(playerLeft[i]);
        al_destroy_bitmap(playerRight[i]);
    }
    
    for(int i = 0; i < 2; i++){
        al_destroy_bitmap(treeImgs[i]);
        al_destroy_bitmap(treeImgs[i]);
        al_destroy_bitmap(treeImgs[i]);
    }
    
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


void loadMap(int* tileList, int tileSize, int rows, int cols, int* colInd, CollisionObject* colObjList, ALLEGRO_BITMAP* treeImg[3], int* plx, int* ply) {
    FILE* fPtr = fopen("assets/map.txt", "r");
    if (!fPtr) {
        printf("Failed to open map.txt\n");
        return;
    }

    int ind = 0;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int value;
            if (fscanf(fPtr, "%d", &value) != 1) {
                printf("failed to read");
                value = 0; // default to empty tile if reading fails
            }

            tileList[ind] = value;

            int x = c * tileSize;
            int y = r * tileSize;

            CollisionObject cobj;
            cobj.PosX = x;
            cobj.PosY = y;
            cobj.ID = ind;

            InteracObject inObj;

            if (value == Tree) {
                strcpy(inObj.showmsg.msg, "The tree is too strong to be cut");

                cobj.Type = Tree;
                cobj.image = treeImg[1];
                cobj.width = al_get_bitmap_width(treeImg[1]);
                cobj.height = al_get_bitmap_height(treeImg[1]);
                cobj.Padx = 40;
                cobj.Padyt = 40;
                cobj.Padyb = 5;

                createCollisionObject(colObjList, colInd, cobj, inObj);
            }
            if (value == CutTree) {
                strcpy(inObj.showmsg.msg, "I can put put some gasoline. Burn it. Then cut it");

                cobj.Type = CutTree;
                cobj.image = treeImg[1];
                cobj.width = al_get_bitmap_width(treeImg[1]);
                cobj.height = al_get_bitmap_height(treeImg[1]);
                cobj.Padx = 40;
                cobj.Padyt = 40;
                cobj.Padyb = 5;
                inObj.phase = 1;

                createCollisionObject(colObjList, colInd, cobj, inObj);
            }

            if(value == Log){
                inObj.interact = 0;
                cobj.Type = Log;
                cobj.image = logImg;
                cobj.width = al_get_bitmap_width(logImg);
                cobj.height = al_get_bitmap_height(logImg);
                cobj.Padx = 10;
                cobj.Padyt = 5;
                cobj.Padyb = 60;

                createCollisionObject(colObjList, colInd, cobj, inObj);
            }

            if(value == Rock){
                inObj.interact = 0;
                cobj.Type = Rock;
                cobj.image = rockImg;
                cobj.width = al_get_bitmap_width(rockImg);
                cobj.height = al_get_bitmap_height(rockImg);
                cobj.Padx = 15;
                cobj.Padyt = 20;
                cobj.Padyb = 100;
                createCollisionObject(colObjList, colInd, cobj, inObj);
            }

            if(value == Pile){
                inObj.interact = 0;
                cobj.Type = Pile;
                cobj.image = snowPileImg;
                cobj.width = al_get_bitmap_width(snowPileImg);
                cobj.height = al_get_bitmap_height(snowPileImg);
                cobj.Padx = 0;
                cobj.Padyt = 100;
                cobj.Padyb = 60;
                createCollisionObject(colObjList, colInd, cobj, inObj);
            }

            if(value == Matchbox){
                cobj.Type = Matchbox;
                cobj.image = matchBoxImg;
                cobj.width = al_get_bitmap_width(matchBoxImg);
                cobj.height = al_get_bitmap_height(matchBoxImg);
                cobj.Padx = 20;
                cobj.Padyt = 0;
                cobj.Padyb = 80;
                inObj.maxUsage = 5;
                inObj.usageRemain = 5;

                createCollisionObject(colObjList, colInd, cobj, inObj);
            }
            if(value == Gasoline){
                cobj.Type = Gasoline;
                cobj.image = gasolineImg;
                cobj.width = al_get_bitmap_width(gasolineImg);
                cobj.height = al_get_bitmap_height(gasolineImg);
                cobj.Padx = 10;
                cobj.Padyt = 5;
                cobj.Padyb = 50;
                inObj.maxUsage = 3;
                inObj.usageRemain = 3;

                createCollisionObject(colObjList, colInd, cobj, inObj);
            }

            if(value == Chainsaw){
                cobj.Type = Chainsaw;
                cobj.image = chainsawImg;
                cobj.width = al_get_bitmap_width(chainsawImg);
                cobj.height = al_get_bitmap_height(chainsawImg);
                cobj.Padx = 20;
                cobj.Padyt = 10;
                cobj.Padyb = 65;
                inObj.maxUsage = 20;
                inObj.usageRemain = 20;

                createCollisionObject(colObjList, colInd, cobj, inObj);

            }

            if(value == Free){
                cobj.Type = Free;
                cobj.image = freeImg;
                cobj.PosX += 40;
                cobj.width = al_get_bitmap_width(freeImg);
                cobj.height = al_get_bitmap_height(freeImg);

                createCollisionObject(colObjList, colInd, cobj, inObj);

            }
            

            if(value == PlayerCord){
                *plx = c * tileSize;
                *ply = r * tileSize;
            }

            ind++;
        }
    }

    fclose(fPtr);
    printf("Loaded map. Collision count: %d\n", *colInd);
}

void sortColObjs(CollisionObject* objs, int currentInd){

    int cMax = 0;
    int cIndex = 0;
    CollisionObject* newArray = (CollisionObject*)malloc(sizeof(CollisionObject) * (currentInd + 10));

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < currentInd; j++){
            CollisionObject cObj = objs[j];
            if(cObj.Type == i){
                newArray[cIndex] = cObj;
                cIndex++;
            }
        }
    }
    printf("count: %d\n", cIndex);
    for(int i = 0; i < cIndex; i++){
        objs[i] = newArray[i];
    }
}

void registerRecord(int data){
    printf("gggg\n");
    FILE* fPtr = fopen("assets/leaderboard.txt","a");

    if(!fPtr){
        printf("file load failed\n");
    }

    fprintf(fPtr, "%d\n", data);

    fclose(fPtr);

}


void createCloud(Cloud* cloudList, int* ind, int camX){


    int w = rand() % (150 + 1 - 100) + 100;
    int h = rand() % (100 + 1 - 60) + 60;
    int y = rand() % (MAP_HEIGHT-100 + 1 - 0) + 0;
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

    for (int i = 0; i < *ind; i++) {
        cloudList[i].posx -= cloudList[i].speed;
    }

    int place = 0;
    int leftBound = camX - 400;  // allow some buffer

    for (int i = 0; i < *ind; i++) {
        Cloud cCloud = cloudList[i];
        if (cCloud.posx > leftBound) {
            cloudList[place] = cCloud;
            place++;
        }
    }

    *ind = place;
}

void updateBlizzard(BlizzardEffect* blizzard){
    blizzard->active = true;
    blizzard->x -= blizzard->speed;
    if(blizzard->x + al_get_bitmap_width(blizzard->image) + 10 <= 0){
        blizzard->active = false;
        blizzard->count = 0;
    }
}

void createCollisionObject(CollisionObject* objList, int* currentIndex, CollisionObject obj, InteracObject interac){
    CollisionObject newObj = obj;
    newObj.interaction = interac;
    objList[*currentIndex] = newObj;
    (*currentIndex)++;
}
void deleteCollisionObject(CollisionObject* objList, int* currentIndex, int ID){
    int ind = 0;
    for(int i = 0; i < *currentIndex; i++){
        if(objList[i].ID == ID)continue;
        objList[ind] = objList[i];
        ind++;
    }

    *currentIndex = ind;

}

bool collisionCheck(int px, int py, int ox, int oy, int pwidth, int pheight, int owidth, int oheight, int opadx, int opadyt, int opadyb){
    if(px > ox + owidth - opadx || px + pwidth < ox + opadx || py > oy + oheight - opadyb || py + pheight < oy + opadyt){
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

void createMessageBox(MessageBox* currentMessages, int* currentIndex, char* message){
    MessageBox newBox;
    strcpy(newBox.msg, message);
    currentMessages[*currentIndex] = newBox;
    (*currentIndex)++;
}
void deleteMessageBox(MessageBox* currentMessages, int* currentIndex) {
    if (*currentIndex <= 0) return;  

    for (int i = 1; i < *currentIndex; i++) {
        currentMessages[i - 1] = currentMessages[i]; 
    }

    (*currentIndex)--;
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
    int min = timeRemaining / 60;
    int secs = timeRemaining % 60;
    snprintf(timerString,20,"00:%02d:%02d",min,secs);
}