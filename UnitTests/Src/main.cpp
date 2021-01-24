#include "app.hpp"


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
	Sprite* mario = player->GetSprite();
	Sprite* goomba= enemy_1->GetSprite();
	switch (al.event.type) {
		case ALLEGRO_EVENT_TIMER:
			if (al.key[ALLEGRO_KEY_UP]) {
				if (!mario->GetGravityHandler().IsFalling()) {
					player->SetDy(-11);
				}
			}
			if (al.key[ALLEGRO_KEY_LEFT]) {
				player->SetDx(-1*player->GetSpeed());
			}
			if (al.key[ALLEGRO_KEY_RIGHT]) {
				player->SetDx(1*player->GetSpeed());
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
	if (!al.key[ALLEGRO_KEY_RIGHT] && !al.key[ALLEGRO_KEY_LEFT]) { player->SetDx(0);}
}
void ai (void) {
	enemy_1->Move();
}
void physics (void) {
	player->Move();
	tlayer->Scroll(*player->GetDx(), *player->GetDy());
	auto list = EntityManager::Get().GetAll();
	for (auto it = list.begin(); it != list.end(); ++it) {
		Sprite* s = (*it)->GetSprite();
		if (s->GetGravityHandler().IsFalling()) {
			(*it)->SetDy(*((*it)->GetDy())+(*it)->GetMass());
			(*it)->SetMass((*it)->GetMass()+(*it)->GetG());
		}
	}
}
void destruct (void) {

}
void collisions (void) {

}
void user (void) {
}
