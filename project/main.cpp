#include "header.hpp"


int main(int argc, char *argv[])
{
    al_init();
    al_install_keyboard();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(WIDTH, HEIGHT);
    ALLEGRO_FONT* font = al_create_builtin_font();

    if (!al_install_keyboard()) {
	    al_show_native_message_box(disp, "Error", "Error", "failed to initialize the keyboard!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
      	return -1;
   	}

    if (!al_init_image_addon()) {
    	al_show_native_message_box(disp, "Error", "Error",
    		"Failed to initialize image addon!", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    	return -1;
	}


    ALLEGRO_BITMAP* player = nullptr;
    player =  al_load_bitmap("frame1.bmp");
    al_convert_mask_to_alpha(player, PINK);

    int dx = 100;
    int dy = 100;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    ALLEGRO_EVENT event;



    if (!player) {
		al_show_native_message_box(disp, "Error", "Error", "Failed to load image!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
      	al_destroy_display(disp);
     	return -1;
	 }

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_TIMER)
            redraw = true;

        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;

        else if (event.type == ALLEGRO_EVENT_KEY_CHAR){
            switch (event.keyboard.keycode)
            {
        case ALLEGRO_EVENT_TIMER:
            if(key[ALLEGRO_KEY_UP])
                dy--;
            if(key[ALLEGRO_KEY_DOWN])
                dy++;
            if(key[ALLEGRO_KEY_LEFT])
                dx--;
            if(key[ALLEGRO_KEY_RIGHT])
                dx++;

            for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= ~KEY_SEEN;

            redraw = true;
            break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= ~KEY_DOWN;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                break;
        }

        }

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");

            al_draw_bitmap(player, dx, dy, 0);

            al_flip_display();

            redraw = false;
        }
    }
    al_destroy_bitmap(player);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}