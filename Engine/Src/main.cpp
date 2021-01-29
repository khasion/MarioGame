#include "../Include/app.hpp"

Game* gaem;

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
	gaem = &game;
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

void gui (void) {
	al_draw_text(al.font,
	al_map_rgb(0, 0, 0), 2, 2, 0,
	("coins: " + std::to_string(player->GetCoins())).c_str()
	);
	al_draw_text(al.font,
	al_map_rgb(0, 0, 0), 315, 2, 0,
	("lives: " + std::to_string(player->GetLives())).c_str()
	);
	al_draw_text(al.font,
	al_map_rgb(0, 0, 0), 520, 2, 0,
	("points: " + std::to_string(player->GetPoints())).c_str()
	);
}

void render (void) {
	if (al_is_event_queue_empty(al.queue)) {
		switch (gaem->GetState()) {
			case START:
				BitmapClear(al_get_target_bitmap(), {0, 0, 0});
				break;
			case PLAY:
				tlayer->Display(al_get_target_bitmap(), Rect {0, 0, 0, 0});
				gui();
				break;
			case END:
				BitmapClear(al_get_target_bitmap(), {0, 0, 0});
				break;
		}
		if (gaem->IsPaused() || gaem->GetState() == START) {
			if (gaem->GetState() == END) {
				al_draw_text(al.font,
				al_map_rgb(255, 255, 255), 50, 120, 0,
				("You scored: " + std::to_string(player->GetPoints())).c_str());
				al_draw_text(al.font,
				al_map_rgb(255, 255, 255), 50, 136, 0,
				"Press ENTER to close the game.");
			}
			else {
				int i = 1;
				for (auto it = messages.begin(); it != messages.end(); ++it) {
					al_draw_text(al.font,
					al_map_rgb(255, 255, 255), 190, 120+(16*i++), 0,
					(*it).c_str());
				}
			}
		}
		al_flip_display();
	}
}

void anim (void) {
	AnimatorManager::GetSingleton().Progress(std::time(nullptr));
}

void input (void) {
	Sprite* mario = player->GetSprite();
	switch (al.event.type) {
		case ALLEGRO_EVENT_TIMER:
			if (al.key[ALLEGRO_KEY_Z]) {
				player->SetDy(-8);
			}
			if (al.key[ALLEGRO_KEY_LEFT]) {
				player->SetDx(-1);
			}
			if (al.key[ALLEGRO_KEY_RIGHT]) {
				player->SetDx(1);
			}
			if (al.key[ALLEGRO_KEY_X]) {
				player->SetDx(*player->GetDx()*player->GetSpeed());
			}
			if (al.key[ALLEGRO_KEY_ESCAPE]) {
				if (!gaem->IsPaused()) {
					gaem->Pause(std::time(nullptr));
				}
			}
			if (al.key[ALLEGRO_KEY_ENTER]) {
				if (gaem->GetState() == END) {
					al.done = true;
				}
				if (gaem->GetState() == START) { 
					gaem->SetState(PLAY);
					player->SetLives(1);
					//AnimatorManager::GetSingleton().TimeShift(gaem->GetGameTime()-gaem->GetPauseTime());
				}
				if (gaem->IsPaused()) {
					gaem->Resume();
					//AnimatorManager::GetSingleton().TimeShift(gaem->GetGameTime()-gaem->GetPauseTime());
				}
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
	auto l = EntityManager::Get().GetAll();
	for (auto it = l.begin(); it != l.end(); ++it) {
		if ((*it).first.compare("player") != 0) {
			if (!(*it).first.compare("piran") ) {
				(*it).second->Do(player->GetSprite());
			}
			else {
				(*it).second->Do();
			}
		}
	}
}
void physics (void) {
	auto list = EntityManager::Get().GetAll();
	for (auto it = list.begin(); it != list.end(); ++it) {
		Entity* entity = (*it).second;
		Sprite* s = entity->GetSprite();
		if (s->GetGravityHandler().IsFalling()) {
			entity->SetDy(*(entity->GetDy())+entity->GetMass()+entity->GetG());
			entity->SetMass(entity->GetMass()+entity->GetG());
			if (*entity->GetDy() > 16) { entity->SetDy(16);}
		}
		else { entity->ResetMass();}
	}
	player->Do();
	tlayer->Scroll(*player->GetDx(), *player->GetDy());
}
void destruct (void) {

}
void collisions (void) {
	CollisionChecker::GetSingleton().Check();
}
void user (void) {
	if ( player->GetSprite()->GetX() == MUL_TILE_WIDTH(181)) {
		gaem->SetState(END);
		gaem->Pause(std::time(nullptr));
	}
	if (player->GetSprite()->GetX() > 1000) {
		player->SetCheck(1000);
	}
	if (player->IsDead()) {
		player->SetDead(false);
		player->GetSprite()->SetPos(
			player->GetCheck(),
			player->GetStartPos().y - 64
		);
		tlayer->SetViewWindow({player->GetCheck() - 320, 0, 640, 480});
		if (player->GetLives() < 0) {
			gaem->SetState(START);
		}
	}
}
