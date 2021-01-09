#include "app.hpp"

int dx = 0, dy = 0;
Rect Player {0, 0, 16, 16};

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
		tlayer->Display(al_get_target_bitmap(), Rect {0, 240, 0, 0});
		al_draw_circle(Player.x, Player.y, 16/2, al_map_rgba_f(0, 0, 0.5, 0.5), 1);
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
				tlayer->Scroll(dx, dy);
			}
			if (al.key[ALLEGRO_KEY_DOWN]) {
				dx = 0 ; dy = 1;
				tlayer->Scroll(dx, dy);
			}
			if (al.key[ALLEGRO_KEY_LEFT]) {
				dx = -1; dy = 0;
				tlayer->Scroll(dx, dy);
			}
			if (al.key[ALLEGRO_KEY_RIGHT]) {
				dx = 1; dy = 0;
				tlayer->Scroll(dx, dy);
			}
			if (al.key[ALLEGRO_KEY_A]) {
				dx = -1; dy = 0;
				tlayer->GetGridLayer()->FilterGridMotion(Player, &dx, &dy);
				Player.x += dx;
			}
			if (al.key[ALLEGRO_KEY_D]) {
				dx = 1; dy = 0;
				tlayer->GetGridLayer()->FilterGridMotion(Player, &dx, &dy);
				Player.x += dx;
			}
			if (al.key[ALLEGRO_KEY_W]) {
				dx = 0; dy = -1;
				tlayer->GetGridLayer()->FilterGridMotion(Player, &dx, &dy);
				Player.y += dy;
			}
			if (al.key[ALLEGRO_KEY_S]) {
				dx = 0; dy = 1;
				tlayer->GetGridLayer()->FilterGridMotion(Player, &dx, &dy);
				Player.y += dy;
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