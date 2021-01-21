#include "app.hpp"

int dx = 0, dy = 0;

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
		tlayer->Display(al_get_target_bitmap(), Rect {0, 0, 0, 0});
		al_flip_display();
	}
}

void anim (void) {

}

void input (void) {
	switch (al.event.type) {
		case ALLEGRO_EVENT_TIMER:
			if (al.key[ALLEGRO_KEY_UP]) {
				dx = 0 ;dy = -1;
				mario->Move(dx, dy);
				tlayer->Scroll(dx, dy);
			}
			if (al.key[ALLEGRO_KEY_DOWN]) {
				dx = 0 ; dy = 1;
				mario->Move(dx, dy);
				tlayer->Scroll(dx, dy);
			}
			if (al.key[ALLEGRO_KEY_LEFT]) {
				dx = -1; dy = 0;
				mario->Move(dx, dy);
				tlayer->Scroll(dx, dy);
			}
			if (al.key[ALLEGRO_KEY_RIGHT]) {
				dx = 1; dy = 0;
				mario->Move(dx, dy);
				tlayer->Scroll(dx, dy);
			}
			if (al.key[ALLEGRO_KEY_A]) {
				dx = -1; dy = 0;
			}
			if (al.key[ALLEGRO_KEY_D]) {
				dx = 1; dy = 0;
			}
			if (al.key[ALLEGRO_KEY_W]) {
				dx = 0; dy = -1;
			}
			if (al.key[ALLEGRO_KEY_S]) {
				dx = 0; dy = 1;
			}
			for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
				al.key[i] &= KEY_SEEN;
			}
			tlayer->SetViewWindow(tlayer->GetViewWindow());
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