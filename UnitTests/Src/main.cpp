#include "app.hpp"

int dx = 0, dy = 0, fw = 1;
float g = 0.1;

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
	AnimatorManager::GetSingleton().Progress(std::time(nullptr));
}

void input (void) {
	switch (al.event.type) {
		case ALLEGRO_EVENT_TIMER:
			if (al.key[ALLEGRO_KEY_UP]) {
				if (!mario->GetGravityHandler().IsFalling()) {
					dy = -5;
					mario->Move(dx, dy);
					tlayer->Scroll(dx, dy);
				}
			}
			if (al.key[ALLEGRO_KEY_LEFT]) {
				dx = -1;
				mario->GetQuantizer().Move(mario->GetBox(), &dx, &dy);
				mario->Move(dx, 0);
				tlayer->Scroll(dx, dy);
			}
			if (al.key[ALLEGRO_KEY_RIGHT]) {
				dx = 1;
				mario->GetQuantizer().Move(mario->GetBox(), &dx, &dy);
				mario->Move(dx, 0);
				tlayer->Scroll(dx, dy);
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
	if (mario->GetGravityHandler().IsFalling()) {
		mario->Move(0, dy+g);
		g += 0.1;
	}
	else { g = 1; dy = 0;}
}
void destruct (void) {

}
void collisions (void) {

}
void user (void) {
}