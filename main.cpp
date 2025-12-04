//Branch Ben_0.1
/*allegro wiki
https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace*/

#include "header.hpp"

#define SLATEGREY   al_map_rgb(112,128,144)

const int SCREEN_W = 640;
const int SCREEN_H = 480;

int main(int argc, char *argv[])
{
    ALLEGRO_DISPLAY *display = nullptr;
	ALLEGRO_EVENT_QUEUE *event_queue = nullptr;

    //initialize allegro
    al_init();

    //initialize display
    display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display) {
    	al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
       	return -1;
	}

    // Initialize keyboard routines
	if (!al_install_keyboard()) {
	    al_show_native_message_box(display, "Error", "Error", "failed to initialize the keyboard!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
      	return -1;
   	}

    // need to add image processor
 	if (!al_init_image_addon()) {
    	al_show_native_message_box(display, "Error", "Error",
    		"Failed to initialize al_init_image_addon!",nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    	return 0;
	}

    // set up event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		al_show_native_message_box(display, "Error", "Error", "Failed to create event_queue!",
                                 nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
      	return -1;
	}

    //register events for event queue
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    
    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    ALLEGRO_BITMAP* mysha = nullptr;
    mysha = al_load_bitmap("mysha.png");

    int dx = 100;
	int dy = 100;
	al_clear_to_color(SLATEGREY);
	al_draw_bitmap(mysha, dx, dy, 0);
	al_flip_display();
	bool doexit = false;
	while (!doexit) {

    	ALLEGRO_EVENT ev;
      	al_wait_for_event(event_queue, &ev);

      	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        	doexit = true;
      	}
      	else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
         	switch(ev.keyboard.keycode) {
            	case ALLEGRO_KEY_UP:
               		dy -= 8;
               		break;
	            case ALLEGRO_KEY_DOWN:
    		        dy += 8;
            		break;
            	case ALLEGRO_KEY_LEFT:
               		dx -= 8;
               		break;
	            case ALLEGRO_KEY_RIGHT:
               		dx += 8;
               		break;
               	case ALLEGRO_KEY_ESCAPE:
               		doexit = true;
               		break;
         	}
         	al_clear_to_color(SLATEGREY);
		 	al_draw_bitmap(mysha, dx, dy, 0);
		 	al_flip_display();
		}
	}

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(mysha);

    return 0;
}