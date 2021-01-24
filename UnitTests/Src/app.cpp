#include "app.hpp"
using namespace app;

Entity* player,*enemy_1;
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
	must_init(al_install_mouse(), "mouse");

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
	al_register_event_source(al.queue, al_get_mouse_event_source());
	memset(al.key, 0, sizeof(al.key));

	tlayer = (new TileLayer(MAX_HEIGHT, MAX_WIDTH, BitmapLoad(SHEET)));

	tlayer->ReadText(TILE_MAP);
	tlayer->GetBg()->LoadBackground(BG_MAP);
	ComputeTileGridBlocks1(tlayer->GetTileMap(), tlayer->GetGridLayer()->GetBuffer());

	AnimationFilmHolder::Get().LoadAll(ANIM_PATH);

	InitPlayer ();
	InitGoomba ();
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
	Sprite* goomba = new Sprite(
		500,
		480-50,
		AnimationFilmHolder::Get().GetAnimationFilm("goomba"),
		"GOOMBA"
	);
	SpriteManager::GetSingleton().Add(goomba);

	goomba->PrepareSpriteGravityHandler(tlayer->GetGridLayer(), goomba);
	goomba->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), goomba));
	goomba->GetGravityHandler().SetOnSolidGround(
	[goomba](const Rect& r) {
		int dx = 0, dy = 1;
		goomba->GetQuantizer().Move(r, &dx, &dy);
		return (!dy) ? true : false;
	});
	goomba->GetGravityHandler().SetOnStartFalling(
	[]() {
		std::cout << "start falling." << std::endl;
	});
	goomba->GetGravityHandler().SetOnStopFalling(
	[]() {
		std::cout << "stop falling." << std::endl;
	});
	enemy_1 = new Entity(goomba, 1, 3, 1);
	enemy_1->SetOnMove ([]() {
			Sprite* goomba = enemy_1->GetSprite();
			int *dx = enemy_1->GetDx(), *dy = enemy_1->GetDy();
			int tempdx = *dx;
			goomba->GetQuantizer().Move(goomba->GetBox(), dx, dy);
			if (*dx == 0) { *dx = tempdx * (-1);}
			goomba->Move(*dx, *dy);
			FrameRangeAnimator* newanimator;
			FrameRangeAnimation* newanimation;
			AnimationFilm* film;
			int start = 0, end = 1;
			std::string curr;
			curr = "goomba";
			newanimator = new FrameRangeAnimator();
			newanimator->SetOnAction(
				[goomba](Animator* animator, const Animation& anim) {
					FrameRange_Action(goomba, animator, (const FrameRangeAnimation&) anim);
				}
			);
			film = AnimationFilmHolder::Get().GetAnimationFilm(curr);
			newanimation = new FrameRangeAnimation(curr, start, end, 100, 0, 0, 1);
			goomba->SetAnimFilm(film);
			player->SetAnimator(newanimator);
			newanimator->Start(newanimation, std::time(nullptr));
		}
	);
	enemy_1->SetDx(1);
	EntityManager::Get().Add(enemy_1);
}
void InitPlayer () {
	Sprite* mario = new Sprite(
		320,
		480-50,
		AnimationFilmHolder::Get().GetAnimationFilm("mario_standl"),
		"MARIO"
	);
	SpriteManager::GetSingleton().Add(mario);

	mario->PrepareSpriteGravityHandler(tlayer->GetGridLayer(), mario);
	mario->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), mario));
	mario->GetGravityHandler().SetOnSolidGround(
	[mario](const Rect& r) {
		int dx = 0, dy = 1;
		mario->GetQuantizer().Move(r, &dx, &dy);
		return (!dy) ? true : false;
	});
	mario->GetGravityHandler().SetOnStartFalling(
	[]() {
		std::cout << "start falling." << std::endl;
	});
	mario->GetGravityHandler().SetOnStopFalling(
	[]() {
		std::cout << "stop falling." << std::endl;
	});


	player = new Entity(mario, 2, 4, 1);
	player->SetOnMove (
		[]() {
			Sprite* mario = player->GetSprite();
			int* dx = player->GetDx(), *dy = player->GetDy();
			mario->GetQuantizer().Move(mario->GetBox(), dx, dy);
			mario->Move(*dx, *dy);

			FrameRangeAnimator* newanimator;
			FrameRangeAnimation* newanimation;
			AnimationFilm* film;
			std::string prev = mario->GetAnimFilm()->GetId();
			std::string curr;
			int start = 0, end = 0;
			bool isFalling = player->GetSprite()->GetGravityHandler().IsFalling();
			if (*dx < 0) {
				curr = "mario_runningl"; end = 1;
				if (isFalling) {curr = "mario_jumpingl"; end = 0;}
			}
			else if (*dx > 0) {
				curr = "mario_runningr"; end = 1;
				if (isFalling) {curr = "mario_jumpingr"; end = 0;}
			}
			else {
				end = 0;
				if (prev.compare("mario_runningr") == 0
				|| prev.compare("mario_jumpingr") == 0 
				|| prev.compare("mario_standr") == 0) {
					curr = "mario_standr";
					if (isFalling) { curr = "mario_jumpingr";}
				}
				else {
					curr = "mario_standl";
					if (isFalling) { curr = "mario_jumpingl";}
				}
			}
			if (curr.compare(prev) == 0) { return;}
			newanimator = new FrameRangeAnimator();
			newanimator->SetOnAction(
				[mario](Animator* animator, const Animation& anim) {
					FrameRange_Action(mario, animator, (const FrameRangeAnimation&) anim);
				}
			);
			film = AnimationFilmHolder::Get().GetAnimationFilm(curr);
			newanimation = new FrameRangeAnimation(curr, start, end, 100, 0, 0, 1);
			mario->SetAnimFilm(film);
			player->SetAnimator(newanimator);
			newanimator->Start(newanimation, std::time(nullptr));
		}
	);
	EntityManager::Get().Add(player);
}
