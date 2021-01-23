#include "app.hpp"
using namespace app;

Entity* player;

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
	player = new Entity(mario, 1);
	player->SetOnMove (
		[](int* dx, int* dy) {
			Sprite* mario = player->GetSprite();
			mario->GetQuantizer().Move(mario->GetBox(), dx, dy);
			mario->Move(*dx, *dy);
			FrameRangeAnimator* newanimator = new FrameRangeAnimator();
			FrameRangeAnimation* newanimation;
			if (*dx < 0) {
				if (*dy != 0) {
					mario->SetAnimFilm(
						AnimationFilmHolder::Get().GetAnimationFilm("mario_jumpingl")
					);
					newanimation = new FrameRangeAnimation("mario_jumpingl", 0, 0, 1, 0, 0, 1);
				}
				else {
					mario->SetAnimFilm(
						AnimationFilmHolder::Get().GetAnimationFilm("mario_runningl")
					);
					newanimation = new FrameRangeAnimation("mario_runningl", 0, 2, 2, *dx, *dy, 1);
				}
			}
			else if (*dx > 0) {
				if (*dy != 0) {
					mario->SetAnimFilm(
						AnimationFilmHolder::Get().GetAnimationFilm("mario_jumpingr")
					);
					newanimation = new FrameRangeAnimation("mario_jumpingr", 0, 0, 1, 0, 0, 1);
				}
				else {
					mario->SetAnimFilm(
						AnimationFilmHolder::Get().GetAnimationFilm("mario_runningr")
					);
					newanimation = new FrameRangeAnimation("mario_runningr", 0, 2, 2, *dx, *dy, 1);
				}
			}
			else {
				if (player->GetSprite()->GetAnimFilm()->GetId().compare("mario_runningr") == 0
				|| player->GetSprite()->GetAnimFilm()->GetId().compare("mario_jumpingr") == 0) {

					mario->SetAnimFilm(
						AnimationFilmHolder::Get().GetAnimationFilm("mario_standr")
					);
					newanimation = new FrameRangeAnimation("mario_standr", 0, 0, 1, 0, 0, 0);
				}
				else {
					mario->SetAnimFilm(
						AnimationFilmHolder::Get().GetAnimationFilm("mario_standl")
					);
					newanimation = new FrameRangeAnimation("mario_standl", 0, 0, 1, 0, 0, 0);
				}
			}
			player->SetAnimator(newanimator);
			newanimator->Start(newanimation, std::time(nullptr));
		}
	);
}