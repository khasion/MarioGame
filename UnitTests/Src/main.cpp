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
			if(enemy_1->GetDx()<100){
				enemy_1->SetDx(enemy_1->GetDx()+1);}
			else if(enemy_1->GetDx()>500){
				enemy_1->SetDx(enemy_1->GetDx()-1);}

			if (al.key[ALLEGRO_KEY_UP]) {
				if (!mario->GetGravityHandler().IsFalling()) {
					dy = -11;
				}
			}
			if (al.key[ALLEGRO_KEY_LEFT]) {
				
				dx = -1;
			}
			if (al.key[ALLEGRO_KEY_RIGHT]) {
				dx = 1;
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
	if (!al.key[ALLEGRO_KEY_RIGHT] && !al.key[ALLEGRO_KEY_LEFT]) { dx = 0;}
}
void ai (void) {

}
void physics (void) {
	int x=enemy_1->GetDx();
	int y=enemy_1->GetDy();
	//enemy_1->Move(&x,&y);
	Sprite* goomba = enemy_1->GetSprite();
	player->Move(&dx, &dy);
	tlayer->Scroll(dx, dy);
	Sprite* mario = player->GetSprite();
	if (mario->GetGravityHandler().IsFalling()) {
		dy = dy + g;
		g += 0.08;
	}
	else { g = 0; dy = 0;}

	if (dy > 16) {dy = 16;}
}
void destruct (void) {

}
void collisions (void) {

}
void user (void) {
}
