#include "../Include/app.hpp"
using namespace app;

Entity* enemy_1, *piranha,*mushroom;;
Mario* player;
Coin** coins;

double g = 0;

void must_init(bool test, const char *description)
{
	if(test) return;

	printf("couldn't initialize %s\n", description);
	exit(1);
}

void App::Initialize (void) {

	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");

	al_install_audio();
	al_init_acodec_addon();

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

	al_reserve_samples(7);
	al.sample = al_load_sample("../../ThirdParty/samples/mario.ogg");
	al.coin_sample = al_load_sample("../../ThirdParty/samples/coin_sample.wav");
	al.jump_sample = al_load_sample("../../ThirdParty/samples/jump_sample.wav");
	al.pipe_sample = al_load_sample("../../ThirdParty/samples/pipe_sample.wav");
	al.power_up_sample = al_load_sample("../../ThirdParty/samples/power_up_sample.wav");
	al.underground_sample = al_load_sample("../../ThirdParty/samples/underground_sample.wav");

	al_register_event_source(al.queue, al_get_keyboard_event_source());
	al_register_event_source(al.queue, al_get_display_event_source(al.disp));
	al_register_event_source(al.queue, al_get_timer_event_source(al.timer));
	memset(al.key, 0, sizeof(al.key));

	tlayer = (new TileLayer(MAX_HEIGHT, MAX_WIDTH, BitmapLoad(SHEET)));

	tlayer->ReadText(TILE_MAP);
	tlayer->GetBg()->LoadBackground(BG_MAP);
	ComputeTileGridBlocks1(tlayer->GetTileMap(), tlayer->GetGridLayer()->GetBuffer());

	AnimationFilmHolder::Get().LoadAll(ANIM_PATH);

	InitPlayer ();
	InitGoomba ();
	InitPiranha ();
	InitKoopas ();
	InitCoinAndBoxes ();
	InitPowerUps();
	InitCollisions ();
}

void App::Clear (void) {
	free(al.monitor);
	al_destroy_font(al.font);
	al_destroy_display(al.disp);
	al_destroy_timer(al.timer);
	al_destroy_event_queue(al.queue);
	al_destroy_sample(al.sample);
	al_destroy_sample(al.coin_sample);
	al_destroy_sample(al.jump_sample);
	al_destroy_sample(al.pipe_sample);
	al_destroy_sample(al.power_up_sample);
}

void InitPowerUps () {
	int i = 0;
	for (auto it = mush_xy.begin(); it != mush_xy.end(); ++it) {
		int x = (*it).first, y = (*it).second;
		EntityManager::Get().Add("mush"+std::to_string(i), new Mushroom(x, y-2, 1, 3, 1));
		i++;
	}
	i = 0;
	for (auto it = up_xy.begin(); it != up_xy.end(); ++it) {
		int x = (*it).first, y = (*it).second;
		EntityManager::Get().Add("oneup"+std::to_string(i), new OneUp(x, y-2, 1, 3, 1));
		i++;
	}
	i = 0;
	for (auto it = star_xy.begin(); it != star_xy.end(); ++it) {
		int x = (*it).first, y = (*it).second;
		EntityManager::Get().Add("star"+std::to_string(i), new Star(x, y-2, 1, 3, 1));
		i++;
	}
	i = 0;
	for (auto it = squirrel_xy.begin(); it != squirrel_xy.end(); ++it) {
		int x = (*it).first, y = (*it).second;
		EntityManager::Get().Add("squirrel"+std::to_string(i), new Squirrel(x, y-2, 1, 3, 1));
		i++;
	}
}

void InitGoomba () {
	enemy_1 = new Goomba(500, 430, 1, 3, 1);
	enemy_1->SetOnDamage (
		[]() {
			enemy_1->SetLives(enemy_1->GetLives()-1);
			if (!enemy_1->GetLives()) {
				player->AddPoints(100);
				SpriteManager::GetSingleton().Remove(enemy_1->GetSprite());
				EntityManager::Get().Remove("goomba1", enemy_1);
			}
		}
	);
	enemy_1->SetDx(-1);
	EntityManager::Get().Add("ai", enemy_1);
}

void InitCoinAndBoxes () {
	int i = 0;
	for (auto it = coins_xy.begin(); it != coins_xy.end(); ++it) {
		int x = (*it).first, y = (*it).second;
		EntityManager::Get().Add("coin"+std::to_string(i), new Coin(x, y));
		i++;
	}
	i = 0;
	for (auto it = boxes_xy.begin(); it != boxes_xy.end(); ++it) {
		int x = (*it).first, y = (*it).second;
		Box* b = new Box(x, y+1, rand()%10 + 1);
		EntityManager::Get().Add("box"+std::to_string(i), b);
		b->SetOnDamage(
			[b](){
				b->SetLives(b->GetLives()-1);
				if (b->GetLives() >= 0) { 
					player->AddCoin();
				}
			}
		);
		i++;
	}
	i = 0;
	for (auto it = checkpoint_xy.begin(); it != checkpoint_xy.end(); ++it) {
		int x = (*it).first, y = (*it).second;
		Coin* c = new Coin(x, y);
		EntityManager::Get().Add("check"+std::to_string(i), c);
		c->GetSprite()->SetVisibility(false);
	}
	i = 0;
	for (auto it = brick_xy.begin(); it != brick_xy.end(); ++it) {
		int x = (*it).first, y = (*it).second;
		Brick* brick = new Brick(x, y+1);
		std::string str = "brick"+std::to_string(i);
		EntityManager::Get().Add(str, brick);
		brick->SetOnDamage(
			[brick, x, y, str] () {
				SpriteManager::GetSingleton().Remove(brick->GetSprite());
				EntityManager::Get().Remove(str, brick);
				GridMap* grid = tlayer->GetGridLayer()->GetBuffer();
				(*grid)[DIV_TILE_HEIGHT(y)][DIV_TILE_WIDTH(x)] = GRID_EMPTY_TILE;
			}
		);
	}
}

void InitPiranha () {
	piranha = new Piranha (57*16+8, 480-130, 1);
	piranha->SetOnDamage (
		[] () {
			piranha->GetSprite()->SetVisibility(false);
			((Piranha*)piranha)->GetColl()->SetVisibility(false);
			player->AddPoints(200);
			SpriteManager::GetSingleton().Remove(piranha->GetSprite());
			EntityManager::Get().Remove("piran", piranha);
			SpriteManager::GetSingleton().Remove( ((Piranha*)piranha)->GetColl());
		}
	);
	Teleporter* tele1 = new Teleporter(57*16, 480-131);
	Teleporter* tele2 = new Teleporter(229*16-8, 19*16);
	tele2->GetSprite()->SetVisibility(false);
	EntityManager::Get().Add("piran", piranha);
	EntityManager::Get().Add("tele1", tele1);
	EntityManager::Get().Add("tele2", tele2);
}

void InitPlayer () {
	player = new Mario(320, 430, 2, 1, 2);
	player->SetOnDamage (
		[]() {
			if (player->IsHit()) { return;}
			player->SetHit(true);
			std::vector<ScrollEntry> entry;
			std::string str = player->GetSprite()->GetAnimFilm()->GetId();
			for (int i = 0; i < 60; i++) {
				entry.push_back({-5, 0, 1}); entry.push_back({5, 0, 1});
			}
			ScrollAnimation* scroll_animation = new ScrollAnimation(str, entry);
			ScrollAnimator* scroll_animator = new ScrollAnimator();
			Sprite* sprite = player->GetSprite();
			scroll_animator->SetOnAction(
				[sprite](Animator* animator, const Animation& anim) {
					Sprite_ScrollAction(sprite, (ScrollAnimator*) animator, &((const ScrollAnimation&) anim));
					const ScrollAnimation* a = &(const ScrollAnimation&)anim;
					ScrollAnimator* an = (ScrollAnimator*) animator;
					tlayer->Scroll(a->GetDx(an->GetCurrRep()), a->GetDy(an->GetCurrRep()));
				}
			);
			scroll_animator->SetOnFinish(
				[] (Animator* a) {
					player->SetHit(false);
					if(player->IsSuper()){
						player->SetSuper(false);
					}
					else
					{
						player->SetLives(player->GetLives()-1);
						player->SetDead(true);
						if(player->GetLives() < 0) {
							player->SetDead(true);
						}
					}
				}
			);
			scroll_animator->Start(scroll_animation, std::time(nullptr));
		}
	);
	EntityManager::Get().Add("player", player);
	player->SetLives(2);
}
void InitKoopas () {
	int i = 0;
	for (auto it = koopa_xy.begin(); it != koopa_xy.end(); ++it) {
		int x = (*it).first, y = (*it).second;
		Koopa* koopa = new Koopa(x, y-32, 2, 2, 1, false);
		std::string str = "koopas"+std::to_string(i);
		EntityManager::Get().Add("koopas"+std::to_string(i), koopa);
		koopa->SetOnDamage (
			[koopa, str] () {
				koopa->SetLives(koopa->GetLives()-1);
				if (!koopa->GetLives()) {
					player->AddPoints(100);
					SpriteManager::GetSingleton().Remove(koopa->GetSprite());
					EntityManager::Get().Remove(str, koopa);
				}
			}
		);
		i++;
	}
	i = 0;
	for (auto it = redkoopa_xy.begin(); it != redkoopa_xy.end(); ++it) {
		int x = (*it).first, y = (*it).second;
		Koopa* koopa = new Koopa(x, y-32, 2, 2, 1, true);
		std::string str = "redkoopas"+std::to_string(i);
		EntityManager::Get().Add(str, koopa);
		koopa->SetOnDamage (
			[koopa, str] () {
				koopa->SetLives(koopa->GetLives()-1);
				if (!koopa->GetLives()) {
					player->AddPoints(100);
					SpriteManager::GetSingleton().Remove(koopa->GetSprite());
					EntityManager::Get().Remove(str, koopa);
				}
			}
		);
		i++;
	}
}

void InitCollisions (void) {
	CollisionChecker::GetSingleton().Register(
		player->GetSprite(),
		enemy_1->GetSprite(),
		[](Sprite* s1, Sprite* s2) {
			if (player->IsHit()
			|| enemy_1->GetLives() <= 0) { return;}
			if ( player->IsInv()) { enemy_1->Damage();}
			if (s1->GetGravityHandler().IsFalling()) {
				enemy_1->Damage();
				player->ResetMass();
				player->SetDy(-12);
			}
			else { player->Damage();}
		});
	CollisionChecker::GetSingleton().Register(
		player->GetSprite(),
		SpriteManager::GetSingleton().GetTypeList("PIRANHA_COLL").front(),
		[](Sprite* s1, Sprite* s2) {
			if (!piranha->GetSprite()->IsVisible()) { return;}
			if (player->IsHit()
			|| piranha->GetLives() <= 0) { return;}
			if (player->IsInv()) { piranha->Damage(); return;}
			if (piranha->GetSprite()->GetFrame() >= piranha->GetSprite()->GetAnimFilm()->GetTotalFrames()-2) { return;}
			player->Damage();
		}
	);
	auto list = EntityManager::Get().GetAll();
	for (auto it = list.begin(); it != list.end(); ++it) {
		if ((*it).first.substr(0, 3).compare("box") == 0) {
			Box* box = (Box*)(*it).second;
			CollisionChecker::GetSingleton().Register (
				player->GetSprite(),
				box->GetSprite(),
				[box](Sprite* s1, Sprite* s2) {
					if (player->GetSprite()->GetGravityHandler().IsFalling()
						&& !box->IsHit()) {
						box->SetHit(true);
						box->Damage();
						std::vector<ScrollEntry> entry;
						entry.push_back({0, -5, 1}); entry.push_back({0, 5, 1});
						entry.push_back({0, 5, 1});
						for (int i = 0; i < 30; i++) {
							entry.push_back({0, 0, 1});
						}
						ScrollAnimation* s = new ScrollAnimation (box->GetSprite()->GetTypeId(), entry);
						ScrollAnimator* animator = new ScrollAnimator();
						animator->SetOnAction(
							[box](Animator* animator, const Animation& anim) {
								Sprite_ScrollAction(box->GetSprite(), (ScrollAnimator*) animator, &((const ScrollAnimation&) anim));
							}
						);
						animator->SetOnFinish(
							[box](Animator* a) {
								box->SetHit(false);
							}
						);
						animator->Start(s, std::time(nullptr));
					}
				}
			);
		}
	}
	for (auto it = list.begin(); it != list.end(); ++it) {
		if ((*it).first.substr(0, 4).compare("coin") == 0) {
			Coin* coin = (Coin*) (*it).second;
			std::string str = (*it).first;
			CollisionChecker::GetSingleton().Register (
				player->GetSprite(),
				coin->GetSprite(),
				[coin, str](Sprite* s1, Sprite* s2) {
					if (!coin->IsDead()) {
						player->AddCoin();
						coin->SetDead(true);
						SpriteManager::GetSingleton().Remove(coin->GetSprite());
						EntityManager::Get().Remove(str, coin);
					}
				}
			);
		}
	}
	Teleporter* tele1 = (Teleporter*)EntityManager::Get().Get("tele1");
	CollisionChecker::GetSingleton().Register (
		player->GetSprite(),
		tele1->GetSprite(),
		[] (Sprite* s1, Sprite* s2) {
			if (al.key[ALLEGRO_KEY_DOWN]) {
				al_play_sample(al.pipe_sample,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
				al_stop_sample(&al.id);
				std::string id = player->GetSprite()->GetAnimFilm()->GetId();
				FrameRangeAnimation* f = new FrameRangeAnimation(id, 0, 0, 1, 0, 1, 2);
				FrameRangeAnimator* a = new FrameRangeAnimator();
				Sprite* sprite = player->GetSprite();
				a->SetOnAction(
					[sprite](Animator* animator, const Animation& anim) {
						sprite->SetPos(sprite->GetX(), sprite->GetY()+1);
						FrameRange_Action(sprite, animator, (const FrameRangeAnimation&) anim);
					}
				);
				a->SetOnFinish(
					[] (Animator* animator) {
						//al_play_sample(al.underground_sample, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, &al.id_2);
						player->GetSprite()->SetPos(210*16, 14*16);
						tlayer->SetViewWindow({210*16-320, 0, 640, 480});
					}
				);
				a->Start(f, std::time(nullptr));
			}
		}
	);
	Coin* tele2 = (Coin*)EntityManager::Get().Get("tele2");
	CollisionChecker::GetSingleton().Register (
		player->GetSprite(),
		tele2->GetSprite(),
		[] (Sprite* s1, Sprite* s2) {
			if (al.key[ALLEGRO_KEY_RIGHT]) {
				//al_stop_sample(&al.id_2);
				al_play_sample(al.sample,1,0,1,ALLEGRO_PLAYMODE_LOOP,&al.id);
				player->GetSprite()->SetPos(133*16, 24*16);
				tlayer->SetViewWindow({133*16-320, 0, 640, 480});
			}
		}
	);
	for (auto it = list.begin(); it != list.end(); ++it) {
		if ((*it).first.substr(0, 5).compare("check") == 0) {
			Coin* check = (Coin*)(*it).second;
			CollisionChecker::GetSingleton().Register (
				player->GetSprite(),
				check->GetSprite(),
				[check] (Sprite* s1, Sprite* s2) {
					player->SetCheck(check->GetStartPos().x);
				}
			);
		}
	}
}