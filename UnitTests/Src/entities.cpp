#include "entities.hpp"

EntityManager EntityManager::singleton;

void Entity::Damage () {
	if (onDeath) onDeath();
}

void Entity::SetAnimator (Animator* anim) {
	if (animator) {
		animator->Stop();
		AnimatorManager::GetSingleton().Cancel(animator);
		free(animator);
	}
	animator = anim;
}

void Mario::Do (void) {
	Sprite* mario = GetSprite();
	int* dx = GetDx(), *dy = GetDy();
	mario->GetQuantizer().Move(mario->GetBox(), dx, dy);
	mario->Move(*dx, *dy);

	std::string prev = mario->GetAnimFilm()->GetId();
	std::string curr;
	int start = 0, end = 0;
	bool isFalling = GetSprite()->GetGravityHandler().IsFalling();
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
	FrameRangeAnimator* newanimator = new FrameRangeAnimator();
	newanimator->SetOnAction(
		[mario](Animator* animator, const Animation& anim) {
			FrameRange_Action(mario, animator, (const FrameRangeAnimation&) anim);
		}
	);
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm(curr);
	FrameRangeAnimation* newanimation = new FrameRangeAnimation(curr, start, end, INT_MAX, 0, 0, 1);
	mario->SetAnimFilm(film);
	SetAnimator(newanimator);
	newanimator->Start(newanimation, std::time(nullptr));
}

void Goomba::Do (void) {
	Sprite* goomba = GetSprite();
	int *dx = GetDx(), *dy = GetDy();
	int tempdx = *dx;
	goomba->GetQuantizer().Move(goomba->GetBox(), dx, dy);
	if (*dx == 0) { *dx = tempdx * (-1);}
	goomba->Move(*dx, *dy);
	if (GetAnimator()) { return;}
	FrameRangeAnimator* newanimator = new FrameRangeAnimator();
	newanimator->SetOnAction(
		[goomba](Animator* animator, const Animation& anim) {
			FrameRange_Action(goomba, animator, (const FrameRangeAnimation&) anim);
		}
	);
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("goomba");
	FrameRangeAnimation* newanimation = new FrameRangeAnimation("goomba", 0, 1, INT_MAX, 0, 0, 1);
	goomba->SetAnimFilm(film);
	SetAnimator(newanimator);
	newanimator->Start(newanimation, std::time(nullptr));
}

void Piranha::Do (Sprite* mario) {
	Sprite* s = GetSprite();
	Sprite* m = mario;
	if (s->GetX() - 16 <= m->GetX()
	&& s->GetX() + s->GetWidth() + 16 >= m->GetX()
	&& s->GetFrame() >= s->GetAnimFilm()->GetTotalFrames()-2) { 
		GetAnimator()->Stop();
	}
	else {
		if (GetAnimator()->HasFinished()) {
			FrameRangeAnimation* f = new FrameRangeAnimation("piran", 0, 9, INT_MAX, 0, -2, 1);
			((FrameRangeAnimator*)GetAnimator())->Start(
				f,
				std::time(nullptr)
			);
			s->SetPos(s->GetX(), starty);
		}
	}
	if (s->GetFrame() == 0) {
		s->SetPos(s->GetX(), starty);
	}
}
void Coin::Do () {
	Sprite* s = GetSprite();
	if (GetAnimator()) { return;}
	FrameRangeAnimation* f = new FrameRangeAnimation("coin", 0, 2, INT_MAX, 0, 0, 1);
	((FrameRangeAnimator*)GetAnimator())->Start(
		f,
		std::time(nullptr)
	);
}

void Mario::Init (void) {
	Sprite* mario = new Sprite(
		startx,
		starty,
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
		}
	);
	mario->GetGravityHandler().SetOnStartFalling(
		[]() {
			std::cout << "start falling." << std::endl;
		}
	);
	mario->GetGravityHandler().SetOnStopFalling(
		[]() {
			std::cout << "stop falling." << std::endl;
		}
	);
	SetSprite(mario);
}

void Goomba::Init (void) {
	Sprite* goomba = new Sprite(
		startx,
		starty,
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
	sprite = goomba;
}

void Piranha::Init (void) {
	Sprite* piranha_sprite  = new Sprite (
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm("piran"),
		"PIRANHA"
	);
	Sprite* piranha_collision = new Sprite (
		startx - 8,
		starty - 2,
		AnimationFilmHolder::Get().GetAnimationFilm("piran"),
		"PIRANHA_COLL"
	);
	piranha_collision->SetVisibility(false);
	SpriteManager::GetSingleton().Add(piranha_collision);
	piranha_sprite->SetHasDirectMotion(true);
	piranha_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), piranha_sprite));
	SpriteManager::GetSingleton().Add(piranha_sprite);

	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("piran");
	FrameRangeAnimation* animation = new FrameRangeAnimation("piran", 0, 9, INT_MAX, 0, -2, 1);
	FrameRangeAnimator*	animator = new FrameRangeAnimator();
	animator->SetOnAction(
		[piranha_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(piranha_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	sprite 	= piranha_sprite;
	coll 		= piranha_collision;
	piranha_sprite->SetAnimFilm(film);
	SetAnimator(animator);
	animator->Start(animation, std::time(nullptr));
}

void Coin::Init (void) {
	Sprite* coin_sprite  = new Sprite (
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm("coin"),
		"COIN"
	);

	SpriteManager::GetSingleton().Add(coin_sprite);
	coin_sprite->SetHasDirectMotion(true);
	coin_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), coin_sprite));
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("coin");
	FrameRangeAnimation* animation = new FrameRangeAnimation("piran", 0, 2, INT_MAX, 0, 0, 1);
	FrameRangeAnimator*	animator = new FrameRangeAnimator();
	animator->SetOnAction(
		[coin_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(coin_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	sprite = coin_sprite;
	coin_sprite->SetAnimFilm(film);
	SetAnimator(animator);
	animator->Start(animation, std::time(nullptr));
}