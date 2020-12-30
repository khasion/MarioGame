#include "app.hpp"

int x= 0, y = 0;

void render (void);
void anim (void);
void input (void);
void ai (void);
void physics (void);
void destruct (void);
void collisions (void);
void user (void);
bool done(void) {
	return al.done;
}

int main()
{
	app::App app;
	Game& game = app.GetGame();
	game.setRender(render);
	game.setAnim(anim);
	game.setInput(input);
	game.setAI(ai);
	game.setPhysics(physics);
	game.setDestruct(destruct);
	game.setCollisions(collisions);
	game.setUser(user);
	game.setDone(done);
	app.Main();

	return 0;
}

void render (void) {
	if (al_is_event_queue_empty(al.queue)) {
		dpyBuffer = al_get_target_bitmap();
		TileTerrainDisplay(
							&tilemap,
							al.map.bitmap,
							Rect{0, 0, GetResWidth(), GetResHeight()},
							Rect{x, y, MAX_WIDTH, MAX_HEIGHT});
		/*for (int i = 0; i < MAX_HEIGHT; i++) {
			for (int j = 0; j < MAX_WIDTH; j++) {
				Index index = GetTile(&tilemap, j, i);
				PutTile(al_get_target_bitmap(), x+j*16, y+i*16, al.map.bitmap, index);
			}
		}*/
		al_flip_display();
	}
}

void anim (void) {

}

void input (void) {
	switch (al.event.type) {
		case ALLEGRO_EVENT_TIMER:
			if (al.key[ALLEGRO_KEY_HOME]) {
				y-=5;
			}
			if (al.key[ALLEGRO_KEY_END]) {
				y+=5;
			}
			for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
				al.key[i] &= KEY_SEEN;
			}
			dpyChanged = true;
			break;
		case ALLEGRO_EVENT_MOUSE_AXES:
			x = al.event.mouse.x;
			dpyChanged = true;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			al.key[al.event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
			break;
		case ALLEGRO_EVENT_KEY_UP:
			al.key[al.event.keyboard.keycode] &= KEY_RELEASED;
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			al.done = true;
			break;
	}
}
void ai (void) {

}
void physics (void) {

}
void destruct (void) {

}
void collisions (void) {

}
void user (void) {
}