#include "app.hpp"
using namespace app;

Entity* player,*enemy_1, *piranha;
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
	InitCoin ();
	InitCollisions ();
}

void App::Clear (void) {
	exit(0);
	AnimationFilmHolder::Get().Destroy();
	std::cout << "EDW TRWEI SEG" << std::endl;
	free(al.monitor);
	al_destroy_font(al.font);
	al_destroy_display(al.disp);
	al_destroy_timer(al.timer);
	al_destroy_event_queue(al.queue);
}

void InitGoomba () {
	enemy_1 = new Goomba(500, 430, 1, 3, 1);
	enemy_1->SetOnDeath (
		[]() {
			enemy_1->SetLives(enemy_1->GetLives()-1);
			if (!enemy_1->GetLives()) {
				SpriteManager::GetSingleton().Remove(enemy_1->GetSprite());
				EntityManager::Get().Remove(enemy_1);
			}
		}
	);
	enemy_1->SetDx(-1);
	EntityManager::Get().Add(enemy_1);
}

void InitCoin () {
	coins = (Coin**) malloc(sizeof(Coin*)*10);
	for (int i=1; i <= 10; i++) {
		coins[i-1] = new Coin(i*16, 480-130);
	}
}

void InitPiranha () {
	piranha = new Piranha (57*16+8, 480-130, 1);
}

void InitPlayer () {
	player = new Mario(320, 430, 2, 1, 1);
	player->SetOnDeath (
		[]() {
			player->SetLives(player->GetLives()-1);
			/*if (!player->GetLives()) {
				SpriteManager::GetSingleton().Remove(player->GetSprite());
				EntityManager::Get().Remove(player);
			}*/
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
			scroll_animator->Start(scroll_animation, std::time(nullptr));
		}
	);
	EntityManager::Get().Add(player);
}

void InitCollisions (void) {
	CollisionChecker::GetSingleton().Register(
		player->GetSprite(),
		enemy_1->GetSprite(),
		[](Sprite* s1, Sprite* s2) {
			if (player->GetLives() <= 0
			|| enemy_1->GetLives() <= 0) { return;}
			if (s1->GetGravityHandler().IsFalling()) {
				enemy_1->Damage();
			}
			else { player->Damage();}
		});
	CollisionChecker::GetSingleton().Register(
		player->GetSprite(),
		SpriteManager::GetSingleton().GetTypeList("PIRANHA_COLL").front(),
		[](Sprite* s1, Sprite* s2) {
			if (player->GetLives() <= 0
			|| piranha->GetLives() <= 0) { return;}
			if (piranha->GetSprite()->GetFrame() >= piranha->GetSprite()->GetAnimFilm()->GetTotalFrames()-2) { return;}
			player->Damage();
		}
	);
}