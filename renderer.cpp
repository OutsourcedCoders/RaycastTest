#include <iostream>
#include "renderer.h"

namespace raycastgame{
	renderer::renderer(ALLEGRO_DISPLAY *ptrdisp) : screenW(al_get_display_width(ptrdisp)), screenH(al_get_display_height(ptrdisp)), disp(ptrdisp) {
		if(screenW < 1 || screenH < 1)
			std::cerr << "Invalid display size.";
	}

	bool renderer::render(){
		al_set_target_backbuffer(disp);
		al_clear_to_color(al_map_rgb(133, 133, 133));
		al_draw_filled_rectangle(0, (screenH - 1) / 2, screenW - 1, screenH - 1, al_map_rgb(33, 33, 33));

		al_flip_display();
	}
}
