#include "app.hpp"
using namespace app;

Sprite* mario;

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

void InitPlayer () {
	AnimationFilm* standingl 	= AnimationFilmHolder::Get().GetAnimationFilm("mario_standl");
	AnimationFilm* standingr	= AnimationFilmHolder::Get().GetAnimationFilm("mario_standr");
	AnimationFilm* runningl 	= AnimationFilmHolder::Get().GetAnimationFilm("mario_runningl");
	AnimationFilm* runningr 	= AnimationFilmHolder::Get().GetAnimationFilm("mario_runningr");
	AnimationFilm* jumpingl 	= AnimationFilmHolder::Get().GetAnimationFilm("mario_jumpingl");
	AnimationFilm* jumpingr 	= AnimationFilmHolder::Get().GetAnimationFilm("mario_jumpingr");

	mario = new Sprite(320, 480-50, runningr, "MARIO");
	SpriteManager::GetSingleton().Add(mario);

	FrameRangeAnimator* mario_animator = new FrameRangeAnimator();
	mario_animator->SetOnAction(
		[](Animator* animator, const Animation& anim) {
			FrameRange_Action(mario, animator, (const FrameRangeAnimation&) anim);
		});
	FrameRangeAnimation* framerange = new FrameRangeAnimation("mario_standl", 0, 1, 100, 0, 0, 1);
	mario_animator->Start(framerange, std::time(nullptr));

	mario->PrepareSpriteGravityHandler(tlayer->GetGridLayer(), mario);
	mario->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), mario));
	mario->GetGravityHandler().SetOnSolidGround([](const Rect& r) {
		int dx = 0, dy = 1;
		mario->GetQuantizer().Move(r, &dx, &dy);
		return (!dy) ? true : false;
	});
	mario->GetGravityHandler().SetOnStartFalling(
	[jumpingl, mario_animator]() {
		mario_animator->Start((FrameRangeAnimation*)jumpingl, std::time(nullptr));
		std::cout << "start falling." << std::endl;
	});
	mario->GetGravityHandler().SetOnStopFalling(
	[standingl, mario_animator]() {
		mario_animator->Start((FrameRangeAnimation*)standingl, std::time(nullptr));
		std::cout << "stop falling." << std::endl;
	});
}