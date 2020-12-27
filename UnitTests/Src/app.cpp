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

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
	must_init(timer, "timer");
	al.timer = timer;

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	must_init(queue, "queue");
	al.queue = queue;

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	ALLEGRO_DISPLAY* disp = al_create_display(DISP_W, DISP_H);
	must_init(disp, "display");
	al.disp = disp;

	ALLEGRO_FONT* font = al_create_builtin_font();
	must_init(font, "font");
	al.font = font;

	must_init(al_init_image_addon(), "image addon");

	must_init(al_init_primitives_addon(), "primitives");
}

void App::Load (void) {
	al.sprites.sheet = al_load_bitmap(SPRITE_SHEET);
	al_register_event_source(al.queue, al_get_keyboard_event_source());
	al_register_event_source(al.queue, al_get_display_event_source(al.disp));
	al_register_event_source(al.queue, al_get_timer_event_source(al.timer));
	al_register_event_source(al.queue, al_get_mouse_event_source());
	memset(al.key, 0, sizeof(al.key));
	
	int cnt = 0, i, j;
	for (i = 0; i < SHEET_H; i+=TILE_H) {
		for (j = 0; j < SHEET_W; j+=TILE_W) {
			ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(al.sprites.sheet, j, i, TILE_W, TILE_H);
			must_init(sprite, "sprite grab");
			al.sprites.sprite_map[cnt++] = sprite;
		}
	}
}

void App::Clear (void) {
	for (int i = 0; i < SPRITE_MAX; i++) {
		al_destroy_bitmap(al.sprites.sprite_map[i]);
	}
	al_destroy_bitmap(al.sprites.sheet);
	al_destroy_font(al.font);
	al_destroy_display(al.disp);
	al_destroy_timer(al.timer);
	al_destroy_event_queue(al.queue);
}