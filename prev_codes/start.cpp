#include "header.hpp"

int main(int argc, char* argv[]) {

    // Initialize Allegro core first
    if (!al_init()) {
        printf("Failed to initialize Allegro!\n");
        return -1;
    }

    // Create display


    ALLEGRO_DISPLAY* display;
    // al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        printf("Failed to create display!\n");
        return -1;
    }

    al_init_ttf_addon();
    al_init_font_addon();
    al_init_primitives_addon();

    ALLEGRO_FONT* font = al_load_ttf_font("assets/fonts/Anta-Regular.ttf", 106, 0);

    // Show a message box
    int resp = al_show_native_message_box(
        display,
        "message",
        "Question",
        "You an idiot?",
        NULL,
        ALLEGRO_MESSAGEBOX_QUESTION | ALLEGRO_MESSAGEBOX_YES_NO
    );

    char txt[100];

    if(resp == 1){
        strcpy(txt, "So you are an idiot");
    }
    if(resp == 2){
        strcpy(txt, "Liar!");
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font, al_map_rgb(53,199,83), SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTER, txt);
    al_draw_ellipse(SCREEN_W/2, SCREEN_H/2 + 80, 600, 200, al_map_rgb(43,10,100),10);
    
    al_flip_display();
    al_rest(5.0);
    al_destroy_font(font);
    al_destroy_display(display);

    return 0;
}
