#include "app.hpp"
using namespace app;

void must_init(bool test, const char *description)
{
	if(test) return;

	printf("couldn't initialize %s\n", description);
	exit(1);
}

void App::Initialize (void) {
	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");
	must_init(al_install_mouse(), "mouse");

	al.timer = al_create_timer(FPS);
	must_init(al.timer, "timer");

	al.queue = al_create_event_queue();
	must_init(al.queue, "queue");

	must_init(Open(DISP_W, DISP_H, bits8), "display");

	al.font = al_create_builtin_font();
	must_init(al.font, "font");

	must_init(al_init_image_addon(), "image addon");

	must_init(al_init_primitives_addon(), "primitives");
}

void App::Load (void) {
	view = {0, 0, GetResWidth(), GetResHeight()};
	al.map.bitmap = (ALLEGRO_BITMAP*) BitmapLoad(SHEET);
	al_register_event_source(al.queue, al_get_keyboard_event_source());
	al_register_event_source(al.queue, al_get_display_event_source(al.disp));
	al_register_event_source(al.queue, al_get_timer_event_source(al.timer));
	al_register_event_source(al.queue, al_get_mouse_event_source());
	memset(al.key, 0, sizeof(al.key));
	
	ReadTextMap(TILE_MAP);
	ComputeTileGridBlocks1(&tilemap, &grid);
	dpyBuffer = BitmapCreate(640, 480);
}

void App::Clear (void) {
	free(al.monitor);
	BitmapDestroy(al.map.bitmap);
	al_destroy_font(al.font);
	al_destroy_display(al.disp);
	al_destroy_timer(al.timer);
	al_destroy_event_queue(al.queue);
}