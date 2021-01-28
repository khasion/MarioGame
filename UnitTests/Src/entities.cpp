#include "entities.hpp"

EntityManager EntityManager::singleton;

void Entity::Damage () {
	if (onDamage) onDamage();
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
	if (IsSuper()) { curr = "super_"+curr;}
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

void Mushroom::Do (void) {
	Sprite* mushroom = GetSprite();
	int *dx = GetDx(), *dy = GetDy();
	int tempdx = *dx;
	mushroom->GetQuantizer().Move(mushroom->GetBox(), dx, dy);
	if (*dx == 0) { *dx = tempdx * (-1);}
	mushroom->Move(*dx, *dy);
	if (GetAnimator()) { return;}
	FrameRangeAnimator* newanimator = new FrameRangeAnimator();
	newanimator->SetOnAction(
		[mushroom](Animator* animator, const Animation& anim) {
			FrameRange_Action(mushroom, animator, (const FrameRangeAnimation&) anim);
		}
	);
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("mushroom");
	FrameRangeAnimation* newanimation = new FrameRangeAnimation("mushroom", 0, 2, INT_MAX, 0, 0, 1);
	mushroom->SetAnimFilm(film);
	SetAnimator(newanimator);
	newanimator->Start(newanimation, std::time(nullptr));
}

void OneUp::Do (void) {
	Sprite* one = GetSprite();
	int *dx = GetDx(), *dy = GetDy();
	int tempdx = *dx;
	one->GetQuantizer().Move(one->GetBox(), dx, dy);
	if (*dx == 0) { *dx = tempdx * (-1);}
	one->Move(*dx, *dy);
	if (GetAnimator()) { return;}
	FrameRangeAnimator* newanimator = new FrameRangeAnimator();
	newanimator->SetOnAction(
		[one](Animator* animator, const Animation& anim) {
			FrameRange_Action(one, animator, (const FrameRangeAnimation&) anim);
		}
	);
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("1up");
	FrameRangeAnimation* newanimation = new FrameRangeAnimation("1up", 0, 0, INT_MAX, 0, 0, 1);
	one->SetAnimFilm(film);
	SetAnimator(newanimator);
	newanimator->Start(newanimation, std::time(nullptr));
}

void Star::Do (void) {
	Sprite* star = GetSprite();
	int *dx = GetDx(), *dy = GetDy();
	int tempdx = *dx;
	star->GetQuantizer().Move(star->GetBox(), dx, dy);
	if (*dx == 0) { *dx = tempdx * (-1);}
	star->Move(*dx, *dy);
	if (GetAnimator()) { return;}
	FrameRangeAnimator* newanimator = new FrameRangeAnimator();
	newanimator->SetOnAction(
		[star](Animator* animator, const Animation& anim) {
			FrameRange_Action(star, animator, (const FrameRangeAnimation&) anim);
		}
	);
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("star");
	FrameRangeAnimation* newanimation = new FrameRangeAnimation("star", 0, 4, INT_MAX, 0, 0, 1);
	star->SetAnimFilm(film);
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

void Koopa::Do (void) {

}

void RedKoopa::Do (void) {

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

void Box::Do () {
	Sprite* s = GetSprite();
	if (GetAnimator()) { return;}
	FrameRangeAnimation* f = new FrameRangeAnimation("boxes", 0, 2, INT_MAX, 0, 0, 1);
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

void Koopa::Init () {
	Sprite* koopa_sprite = new Sprite(
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm("koopal"),
		"KOOPA"
	);
	SpriteManager::GetSingleton().Add(koopa_sprite);

	koopa_sprite->PrepareSpriteGravityHandler(tlayer->GetGridLayer(), koopa_sprite);
	koopa_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), koopa_sprite));
	koopa_sprite->GetGravityHandler().SetOnSolidGround(
	[koopa_sprite](const Rect& r) {
		int dx = 0, dy = 1;
		koopa_sprite->GetQuantizer().Move(r, &dx, &dy);
		return (!dy) ? true : false;
	});
	koopa_sprite->GetGravityHandler().SetOnStartFalling(
	[]() {
		std::cout << "start falling." << std::endl;
	});
	koopa_sprite->GetGravityHandler().SetOnStopFalling(
	[]() {
		std::cout << "stop falling." << std::endl;
	});
	sprite = koopa_sprite;
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("koopal");
	FrameRangeAnimation* animation = new FrameRangeAnimation("koopal", 0, 1, INT_MAX, 0, 0, 1);
	FrameRangeAnimator*	animator = new FrameRangeAnimator();
	animator->SetOnAction(
		[koopa_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(koopa_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	koopa_sprite->SetAnimFilm(film);
	SetAnimator(animator);
	animator->Start(animation, std::time(nullptr));
}

void RedKoopa::Init (void) {
	Sprite* redkoopa_sprite = new Sprite(
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm("redkoopal"),
		"REDKOOPA"
	);
	SpriteManager::GetSingleton().Add(redkoopa_sprite);

	redkoopa_sprite->PrepareSpriteGravityHandler(tlayer->GetGridLayer(), redkoopa_sprite);
	redkoopa_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), redkoopa_sprite));
	redkoopa_sprite->GetGravityHandler().SetOnSolidGround(
	[redkoopa_sprite](const Rect& r) {
		int dx = 0, dy = 1;
		redkoopa_sprite->GetQuantizer().Move(r, &dx, &dy);
		return (!dy) ? true : false;
	});
	redkoopa_sprite->GetGravityHandler().SetOnStartFalling(
	[]() {
		std::cout << "start falling." << std::endl;
	});
	redkoopa_sprite->GetGravityHandler().SetOnStopFalling(
	[]() {
		std::cout << "stop falling." << std::endl;
	});
	sprite = redkoopa_sprite;
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("redkoopal");
	FrameRangeAnimation* animation = new FrameRangeAnimation("redkoopal", 0, 1, INT_MAX, 0, 0, 1);
	FrameRangeAnimator*	animator = new FrameRangeAnimator();
	animator->SetOnAction(
		[redkoopa_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(redkoopa_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	redkoopa_sprite->SetAnimFilm(film);
	SetAnimator(animator);
	animator->Start(animation, std::time(nullptr));
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
	FrameRangeAnimation* animation = new FrameRangeAnimation("coin", 0, 2, INT_MAX, 0, 0, 1);
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

void Mushroom::Init (void) {
	Sprite* mush_sprite  = new Sprite (
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm("mushroom"),
		"MUSHROOM"
	);
	mush_sprite->SetVisibility(false);
	mush_sprite->PrepareSpriteGravityHandler(tlayer->GetGridLayer(), mush_sprite);

	mush_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), mush_sprite));
	mush_sprite->GetGravityHandler().SetOnSolidGround(
	[mush_sprite](const Rect& r) {
		int dx = 0, dy = 1;
		mush_sprite->GetQuantizer().Move(r, &dx, &dy);
		return (!dy) ? true : false;
	});
	mush_sprite->GetGravityHandler().SetOnStartFalling(
	[]() {
		std::cout << "start falling." << std::endl;
	});
	mush_sprite->GetGravityHandler().SetOnStopFalling(
	[]() {
		std::cout << "stop falling." << std::endl;
	});

	SpriteManager::GetSingleton().Add(mush_sprite);

	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("mushroom");
	FrameRangeAnimation* animation = new FrameRangeAnimation("mushroom", 0, 0, INT_MAX, 0, 0, 1);
	FrameRangeAnimator*	animator = new FrameRangeAnimator();
	animator->SetOnAction(
		[mush_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(mush_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	sprite = mush_sprite;
	mush_sprite->SetAnimFilm(film);
	SetAnimator(animator);
	animator->Start(animation, std::time(nullptr));
	
	auto list = EntityManager::Get().GetAll();
	Mushroom* mush = this;
	for (auto it = list.begin(); it != list.end(); ++it) {
		if ((*it).first.substr(0, 3).compare("box") != 0) { continue;}
		Box* box = (Box*)(*it).second;
		CollisionChecker::GetSingleton().Register(
			GetSprite(),
			box->GetSprite(),
			[mush] (Sprite* s1, Sprite* s2) {
				if (*mush->GetDx() == 0) { 
					mush->GetSprite()->SetVisibility(true);
					mush->SetDx(1);
				}
				else { mush->SetDx(*mush->GetDx()*(-1));}
			}
		);
	}
	Mario* mario = (Mario*)EntityManager::Get().Get("player");
	CollisionChecker::GetSingleton().Register(
		GetSprite(),
		mario->GetSprite(),
		[mario, mush] (Sprite* s1, Sprite * s2) {
			if (!mush->IsDead()) {
				mush->SetDead(true);
				mario->SetSuper(true);
				mario->SetDy(-12);
				SpriteManager::GetSingleton().Remove(mush->GetSprite());
			}
		}
	);
}

void OneUp::Init () {
	Sprite* one_sprite  = new Sprite (
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm("1up"),
		"1UP"
	);
	one_sprite->SetVisibility(false);
	one_sprite->PrepareSpriteGravityHandler(tlayer->GetGridLayer(), one_sprite);

	one_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), one_sprite));
	one_sprite->GetGravityHandler().SetOnSolidGround(
	[one_sprite](const Rect& r) {
		int dx = 0, dy = 1;
		one_sprite->GetQuantizer().Move(r, &dx, &dy);
		return (!dy) ? true : false;
	});
	one_sprite->GetGravityHandler().SetOnStartFalling(
	[]() {
		std::cout << "start falling." << std::endl;
	});
	one_sprite->GetGravityHandler().SetOnStopFalling(
	[]() {
		std::cout << "stop falling." << std::endl;
	});

	SpriteManager::GetSingleton().Add(one_sprite);

	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("1up");
	FrameRangeAnimation* animation = new FrameRangeAnimation("1up", 0, 0, INT_MAX, 0, 0, 1);
	FrameRangeAnimator*	animator = new FrameRangeAnimator();
	animator->SetOnAction(
		[one_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(one_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	sprite = one_sprite;
	one_sprite->SetAnimFilm(film);
	SetAnimator(animator);
	animator->Start(animation, std::time(nullptr));
	
	auto list = EntityManager::Get().GetAll();
	OneUp* one = this;
	for (auto it = list.begin(); it != list.end(); ++it) {
		if ((*it).first.substr(0, 3).compare("box") != 0) { continue;}
		Box* box = (Box*)(*it).second;
		CollisionChecker::GetSingleton().Register(
			GetSprite(),
			box->GetSprite(),
			[one] (Sprite* s1, Sprite* s2) {
				if (*one->GetDx() == 0) { 
					one->GetSprite()->SetVisibility(true);
					one->SetDx(1);
				}
				else { one->SetDx(*one->GetDx()*(-1));}
			}
		);
	}
	Mario* mario = (Mario*)EntityManager::Get().Get("player");
	CollisionChecker::GetSingleton().Register(
		GetSprite(),
		mario->GetSprite(),
		[mario, one] (Sprite* s1, Sprite * s2) {
			if (!one->IsDead()) {
				one->SetDead(true);
				mario->SetLives(mario->GetLives()+1);
				SpriteManager::GetSingleton().Remove(one->GetSprite());
			}
		}
	);
}

void Star::Init (void) {
	Sprite* star_sprite  = new Sprite (
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm("star"),
		"STAR"
	);
	star_sprite->SetVisibility(false);
	star_sprite->PrepareSpriteGravityHandler(tlayer->GetGridLayer(), star_sprite);

	star_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), star_sprite));
	star_sprite->GetGravityHandler().SetOnSolidGround(
	[star_sprite](const Rect& r) {
		int dx = 0, dy = 1;
		star_sprite->GetQuantizer().Move(r, &dx, &dy);
		return (!dy) ? true : false;
	});
	star_sprite->GetGravityHandler().SetOnStartFalling(
	[]() {
		std::cout << "start falling." << std::endl;
	});
	star_sprite->GetGravityHandler().SetOnStopFalling(
	[]() {
		std::cout << "stop falling." << std::endl;
	});

	SpriteManager::GetSingleton().Add(star_sprite);

	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("star");
	FrameRangeAnimation* animation = new FrameRangeAnimation("star", 0, 4, INT_MAX, 0, 0, 1);
	FrameRangeAnimator*	animator = new FrameRangeAnimator();
	animator->SetOnAction(
		[star_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(star_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	sprite = star_sprite;
	star_sprite->SetAnimFilm(film);
	SetAnimator(animator);
	animator->Start(animation, std::time(nullptr));
	
	auto list = EntityManager::Get().GetAll();
	Star* star = this;
	for (auto it = list.begin(); it != list.end(); ++it) {
		if ((*it).first.substr(0, 3).compare("box") != 0) { continue;}
		Box* box = (Box*)(*it).second;
		CollisionChecker::GetSingleton().Register(
			GetSprite(),
			box->GetSprite(),
			[star] (Sprite* s1, Sprite* s2) {
				if (*star->GetDx() == 0) { 
					star->GetSprite()->SetVisibility(true);
					star->SetDx(1);
				}
				else { star->SetDx(*star->GetDx()*(-1));}
			}
		);
	}
}

void Box::Init (void) {
	Sprite* box_sprite  = new Sprite (
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm("boxes"),
		"BOX"
	);

	SpriteManager::GetSingleton().Add(box_sprite);
	box_sprite->SetHasDirectMotion(true);
	box_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), box_sprite));
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("boxes");
	FrameRangeAnimation* animation = new FrameRangeAnimation("boxes", 0, 2, INT_MAX, 0, 0, 1);
	FrameRangeAnimator*	animator = new FrameRangeAnimator();
	animator->SetOnAction(
		[box_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(box_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	sprite = box_sprite;
	box_sprite->SetAnimFilm(film);
	SetAnimator(animator);
	animator->Start(animation, std::time(nullptr));
}