#include <iostream>
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <tinyxml2.h>
#include <zlib.h>

const int mapWidth = 20;
const int mapHeight = 20;
const int displayW = 640;
const int displayH = 480;
const int textureSize = 16;
enum KEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SHIFT
};
enum AXIS{
	MOUSE_X, DELTA_MOUSE_X, OLD_MOUSE_X, OLD_DELTA_MOUSE_X
};

int worldMap[mapWidth][mapHeight] = {
	{1,2,2,3,1,1,1,1,1,1,1,2,2,3,1,1,1,1,1,1},
	{3,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{3,0,0,0,0,1,0,0,0,1,0,0,3,0,0,0,0,0,0,2},
	{1,0,0,0,0,0,0,0,0,1,0,0,3,0,0,0,0,0,0,3},
	{1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,4},
	{1,0,0,2,1,0,0,0,0,1,0,0,3,1,1,1,1,0,1,5},
	{1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,3,0,0,1,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1},
	{1,0,0,0,1,0,0,0,0,1,0,0,3,1,1,1,0,0,0,1},
	{1,0,0,3,1,1,1,1,1,1,0,0,0,1,0,1,0,1,1,1},
	{3,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1},
	{3,0,0,0,0,1,0,0,0,0,0,0,3,1,0,0,0,0,0,2},
	{1,0,0,0,0,0,0,0,0,1,0,0,3,1,1,1,1,1,0,3},
	{1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,4},
	{1,0,0,2,1,0,0,0,0,1,0,0,3,1,1,1,0,1,0,5},
	{1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1,1},
	{1,3,0,0,1,0,0,0,0,1,0,0,3,1,0,1,1,1,0,1},
	{1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
	{1,3,0,0,1,0,0,0,0,1,0,0,3,1,1,1,0,1,0,1},
	{1,2,1,3,1,1,1,1,1,1,2,2,3,1,1,1,1,1,1,1}
};

int main(){

	if(!al_init())
		std::cerr << "I couldn't initialize Allegro 5!";
	if(!al_install_keyboard())
		std::cerr << "I couldn't install an appropriate keyboard driver!";
	if(!al_install_mouse())
		std::cerr << "I couldn't install an appropriate mouse driver!";
	if(!al_init_primitives_addon())
		std::cerr << "I couln't initialize the primitives addon!";

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	ALLEGRO_DISPLAY* disp = al_create_display(displayW, displayH);

	if(!timer)
		std::cerr << "I couldn't create a timer!";
	if(!event_queue)
		std::cerr << "I couldn't create an event queue!";
	if(!disp)
		std::cerr << "I couldn't create a display!";

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(disp));

	al_start_timer(timer);
	ALLEGRO_EVENT event;
	bool redraw = true;
	bool stop = false;

	double posX = 3, posY = 2;
	double dirX = -1, dirY = 0;
	double planeX = 0, planeY = .66;
	double time = 0, oldTime = 0;
	double frameTime, moveSpeed, rotSpeed;

	bool key[5] = {false, false, false, false, false};
	int axis[4] = {0, 0, 0, 0};
	ALLEGRO_BITMAP* textures[8];

	//ALLEGRO_BITMAP* backbuffer = al_create_bitmap(displayW, displayH);
	al_hide_mouse_cursor(disp);
	al_set_mouse_axis(0, (displayW - 1) / 2);
	al_grab_mouse(disp);
	while(true){
		al_wait_for_event(event_queue, &event);
		al_grab_mouse(disp);
		switch(event.type){
		case ALLEGRO_EVENT_TIMER:
			oldTime = time;
			time = al_get_timer_count(timer);
			frameTime = (time - oldTime) * 60.0;
			moveSpeed = .05;
			if(key[KEY_SHIFT])
				moveSpeed *= 2;
			rotSpeed = .015 * std::abs(axis[DELTA_MOUSE_X]);

			if(key[KEY_UP]){
				if(worldMap[(int)(posX + dirX * moveSpeed)][(int)posY] == 0)
					posX += dirX * moveSpeed;
				if(worldMap[(int)posX][(int)(posY + dirY * moveSpeed)] == 0)
					posY += dirY * moveSpeed;
			} else if(key[KEY_DOWN]){
				if(worldMap[(int)(posX - dirX * moveSpeed)][(int)posY] == 0)
					posX -= dirX * moveSpeed;
				if(worldMap[(int)posX][(int)(posY - dirY * moveSpeed)] == 0)
					posY -= dirY * moveSpeed;
			}
			if(key[KEY_LEFT]){
				if(worldMap[(int)(posX - planeX * moveSpeed)][(int)posY] == 0)
					posX -= planeX * moveSpeed;
				if(worldMap[(int)posX][(int)(posY - planeY * moveSpeed)] == 0)
					posY -= planeY * moveSpeed;
			} else if(key[KEY_RIGHT]){
				if(worldMap[(int)(posX + planeX * moveSpeed)][(int)posY] == 0)
					posX += planeX * moveSpeed;
				if(worldMap[(int)posX][(int)(posY + planeY * moveSpeed)] == 0)
					posY += planeY * moveSpeed;
			}
			if(axis[MOUSE_X] < axis[OLD_MOUSE_X]){
				double oldDirX = dirX;
				dirX = dirX * std::cos(rotSpeed) - dirY * std::sin(rotSpeed);
				dirY = oldDirX * std::sin(rotSpeed) + dirY * std::cos(rotSpeed);
				double oldPlaneX = planeX;
				planeX = planeX * std::cos(rotSpeed) - planeY * std::sin(rotSpeed);
				planeY = oldPlaneX * std::sin(rotSpeed) + planeY * std::cos(rotSpeed);
			} else if(axis[MOUSE_X] > axis[OLD_MOUSE_X]){
				double oldDirX = dirX;
				dirX = dirX * std::cos(-rotSpeed) - dirY * std::sin(-rotSpeed);
				dirY = oldDirX * std::sin(-rotSpeed) + dirY * std::cos(-rotSpeed);
				double oldPlaneX = planeX;
				planeX = planeX * std::cos(-rotSpeed) - planeY * std::sin(-rotSpeed);
				planeY = oldPlaneX * std::sin(-rotSpeed) + planeY * std::cos(-rotSpeed);
			}


			axis[OLD_MOUSE_X] = axis[MOUSE_X];
			axis[OLD_DELTA_MOUSE_X] = axis[DELTA_MOUSE_X];
			redraw = true;
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			stop = true;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
				stop = true;
				break;
			}

			switch(event.keyboard.keycode){
			case ALLEGRO_KEY_UP:
			case ALLEGRO_KEY_W:
				key[KEY_UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
			case ALLEGRO_KEY_S:
				key[KEY_DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
			case ALLEGRO_KEY_A:
				key[KEY_LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
			case ALLEGRO_KEY_D:
				key[KEY_RIGHT] = true;
				break;
			case ALLEGRO_KEY_LSHIFT:
			case ALLEGRO_KEY_RSHIFT:
				key[KEY_SHIFT] = true;
				break;
			}
			break;
		case ALLEGRO_EVENT_KEY_UP:
			switch(event.keyboard.keycode){
			case ALLEGRO_KEY_UP:
			case ALLEGRO_KEY_W:
				key[KEY_UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
			case ALLEGRO_KEY_S:
				key[KEY_DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
			case ALLEGRO_KEY_A:
				key[KEY_LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
			case ALLEGRO_KEY_D:
				key[KEY_RIGHT] = false;
				break;
			case ALLEGRO_KEY_LSHIFT:
			case ALLEGRO_KEY_RSHIFT:
				key[KEY_SHIFT] = false;
				break;
			}
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			axis[MOUSE_X] = event.mouse.x;
			axis[DELTA_MOUSE_X] = event.mouse.dx;
			if(axis[MOUSE_X] == displayW - 1){
				al_set_mouse_xy(disp, 1, displayH / 2);
				axis[MOUSE_X] = 1;
				axis[DELTA_MOUSE_X] = 0;
			}
			else if(axis[MOUSE_X] == 0){
				al_set_mouse_xy(disp, displayW - 1, displayH / 2);
				axis[MOUSE_X] = displayW - 2;
				axis[DELTA_MOUSE_X] = 0;
			}
			//std::cout << axis[MOUSE_X] << " " << axis[DELTA_MOUSE_X] << std::endl;
		}

		if(stop)
			break;

		if(redraw && al_is_event_queue_empty(event_queue)){
			al_set_target_backbuffer(disp);
			al_clear_to_color(al_map_rgb(133, 133, 133));
			al_draw_filled_rectangle(0, (displayH - 1) / 2, displayW - 1, displayH - 1, al_map_rgb(33, 33, 33));
			for(int x = 0; x < displayW; x++){
                double cameraX = 2 * x / (double)displayW - 1;
                double rayDirX = dirX + planeX * cameraX;
                double rayDirY = dirY + planeY * cameraX;

                //current position
                int mapX = (int)posX;
                int mapY = (int)posY;

                //length of ray from current position
                double sideDistX, sideDistY;

                //length of ray from one side to another side
                double deltaDistX = std::abs(1 / rayDirX);
                double deltaDistY = std::abs(1 / rayDirY);
                double perpWallDist;

                int stepX, stepY;

                int hit = 0;
                int side;

                if(rayDirX < 0){
					stepX = -1;
					sideDistX = (posX - mapX) * deltaDistX;
                } else{
                	stepX = 1;
                	sideDistX = (mapX + 1.0 - posX) * deltaDistX;
                }

                if(rayDirY < 0){
                    stepY = -1;
                    sideDistY = (posY - mapY) * deltaDistY;
                } else{
					stepY = 1;
					sideDistY = (mapY + 1.0 - posY) * deltaDistY;
                }

                while(hit == 0){
					if(sideDistX < sideDistY){
						sideDistX += deltaDistX;
						mapX += stepX;
						side = 0;
					}else{
						sideDistY += deltaDistY;
						mapY += stepY;
						side = 1;
					}

					if(worldMap[mapX][mapY] > 0) hit = 1;
                }

                if(side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
                else		  perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

                int lineHeight = (int)(displayH / perpWallDist);

                int drawStart = -lineHeight / 2 + displayH / 2;
                int drawStartPercent = (-lineHeight / 2.0 / displayH) * 100;
                std::cout << drawStartPercent << ", ";
                if(drawStart < 0) drawStart = 0;
                int drawEnd = lineHeight / 2 + displayH / 2;
                int drawEndPercent =  (lineHeight / 2.0 / displayH) * 100;
                std::cout << drawEndPercent << std::endl;
                if(drawEnd >= displayH) drawEnd = displayH - 1;

                int textureID = worldMap[mapX][mapY] - 1;

                double wallX;
                if(side == 0) wallX = posY + perpWallDist * rayDirY;
                else		  wallX = posX + perpWallDist * rayDirX;
                wallX -= std::floor(wallX);

                int textureX = (int)(wallX * (double)textureSize);
                if(side == 0 && rayDirX > 0)
					textureX = textureSize - textureX - 1;
				if(side == 1 && rayDirY < 0)
					textureX = textureSize - textureX - 1;



                ALLEGRO_COLOR color;
                switch(worldMap[mapX][mapY]){
                	case 1:  color = al_map_rgb(29, 206, 29); break;
                	case 2:  color = al_map_rgb(41, 171, 41); break;
                	case 3:  color = al_map_rgb(45, 136, 45); break;
                	case 4:  color = al_map_rgb(43, 101, 43); break;
                	default: color = al_map_rgb(35, 66, 35); break;
                }
                double shade;

                if(lineHeight > 300) shade = 1;
				else
					shade = std::pow(lineHeight, 2) / std::pow(300.0, 2);

				shade /= -1.0;
				shade += 1.0;
                unsigned char r, g, b;
                al_unmap_rgb(color, &r, &g, &b);
                r -= shade * r;
                g -= shade * g;
                b -= shade * b;
                color = al_map_rgb(r, g, b);
                al_draw_line(x, drawStart, x, drawEnd, color, 1);
			}
			al_flip_display();
		}
	}

	al_ungrab_mouse();
	al_show_mouse_cursor(disp);
    std::cout << "Hello world!" << std::endl;
    al_destroy_display(disp);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);

    return 0;
}
