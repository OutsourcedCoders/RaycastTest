#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <tinyxml2.h>
#include <zlib.h>
#include "renderer.h"

const int mapWidth = 20;
const int mapHeight = 20;
const int displayW = 640;
const int displayH = 480;
const int textureSize = 16;

enum KEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPRINT, KEY_INVENTORY
};
enum AXIS{
	MOUSE_X, DELTA_MOUSE_X, OLD_MOUSE_X, OLD_DELTA_MOUSE_X
};

/*
int worldMap[mapWidth][mapHeight] = {
	{1,2,2,3,1,1,1,1,1,1,1,2,2,3,1,1,1,1,1,1},
	{3,0,0,0,2,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{3,0,0,0,3,0,0,0,0,1,0,0,3,0,0,0,0,0,0,2},
	{1,0,0,0,0,0,0,0,0,1,0,0,3,0,0,0,0,0,0,3},
	{1,0,0,0,1,1,2,0,3,2,0,0,1,0,0,0,0,0,0,4},
	{1,1,4,2,1,0,0,0,0,1,0,0,3,1,1,1,1,0,1,5},
	{1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1},
	{1,0,0,0,1,0,0,0,0,1,0,0,3,1,1,1,0,0,0,1},
	{1,2,0,3,1,1,1,0,1,1,0,0,0,1,0,1,0,1,1,1},
	{3,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1},
	{3,0,0,0,0,1,0,0,0,0,0,0,3,1,0,0,0,0,0,2},
	{1,0,0,0,0,0,0,0,0,1,0,0,3,1,1,1,1,1,0,3},
	{1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,4},
	{1,3,2,2,1,2,0,0,0,1,0,0,3,1,1,1,0,1,0,5},
	{1,0,0,0,0,3,0,0,0,1,0,0,0,1,0,0,0,1,1,1},
	{1,0,0,0,0,4,0,0,0,1,0,0,3,1,0,1,1,1,0,1},
	{1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,2,0,0,0,1,0,0,3,1,1,1,0,1,0,1},
	{1,2,1,3,1,1,1,1,1,1,2,2,3,1,1,1,1,1,1,1}
};
*/

int* worldMap[20];
int** conversion(ALLEGRO_BITMAP* gamemap){
	std::srand((unsigned)time(nullptr));
	int mapw = al_get_bitmap_width(gamemap);
	int maph = al_get_bitmap_height(gamemap);
	//if(mapw != maph || mapw != mapWidth)
		//return worldMap;

	al_lock_bitmap(gamemap, 0, ALLEGRO_LOCK_READONLY);
	for(int i = 0; 20 > i; i++)
		worldMap[i] = new int[20];
	for(int y = 0; mapw > y; y++){
		for(int x = 0; maph > x; x++){
			unsigned char r, g, b;
			ALLEGRO_COLOR color = al_get_pixel(gamemap, x, y);
			int currentpixel;
			al_unmap_rgb(al_get_pixel(gamemap, x, y), &r, &g, &b);
			currentpixel =2;
			//switch()
			if(r == 255 && g == 255 && b == 255)
				currentpixel = 0;
			worldMap[y][x] = currentpixel;
		}
	}
	al_unlock_bitmap(gamemap);
	return worldMap;
}

int main(){

	if(!al_init())
		std::cerr << "I couldn't initialize Allegro 5!";
	if(!al_install_keyboard())
		std::cerr << "I couldn't install an appropriate keyboard driver!";
	if(!al_install_mouse())
		std::cerr << "I couldn't install an appropriate mouse driver!";
	if(!al_init_primitives_addon())
		std::cerr << "I couldn't initialize the primitives addon!";
	if(!al_init_image_addon())
		std::cerr << "I couldn't initialize the image addon!";

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	ALLEGRO_DISPLAY* disp = al_create_display(displayW, displayH);

	if(!timer)
		std::cerr << "I couldn't create a timer!";
	if(!event_queue)
		std::cerr << "I couldn't create an event queue!";
	if(!disp)
		std::cerr << "I couldn't create a display!";

	ALLEGRO_BITMAP* testmap = al_load_bitmap("testmap.png");

	if(!testmap)
		std::cerr << "I couldn't load the test image 'testmap.png'!";

	raycastgame::renderer renderer(disp);

	conversion(testmap);
	for (int y = 0; 20 > y; y++){
		for(int x = 0; 20 > x; x++)
			std::cout << worldMap[y][x] << " ";
		std::cout << std::endl;
	}

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(disp));

	al_start_timer(timer);
	ALLEGRO_EVENT event;

	bool redraw = true;
	bool stop = false;
	bool key[6] = {false, false, false, false, false, false};
	int axis[4] = {0, 0, 0, 0};

	int playerUnitHeight = 32;
	int wallUnitHeight = 64;



	ALLEGRO_BITMAP* textures[8];
	//al_hide_mouse_cursor(disp);
	//al_set_mouse_axis(0, (displayW - 1) / 2);
	//al_grab_mouse(disp);
	while(true){
		al_wait_for_event(event_queue, &event);
		//al_grab_mouse(disp);
		switch(event.type){
		case ALLEGRO_EVENT_KEY_DOWN:
			if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
				stop = true;
				break;
			}
			break;
		}

		if(stop)
			break;

		if(redraw && al_is_event_queue_empty(event_queue)){
			if(renderer.render())
				std::cerr << "um we" << std::endl;
		}
	}

	//al_ungrab_mouse();
	//al_show_mouse_cursor(disp);
    std::cout << "Hello world!" << std::endl;
    al_destroy_display(disp);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_bitmap(testmap);
	std::cout << "something broke if you can't read this." << std::endl;
    return 0;
}
