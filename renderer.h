#ifndef RENDERER_H
#define RENDERER_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

namespace raycastgame{
	class renderer{
		int screenW, screenH;
		ALLEGRO_DISPLAY **disp;

	public:
		renderer(ALLEGRO_DISPLAY &disp);

		bool render();
	};
}

#endif // RENDERER_H
