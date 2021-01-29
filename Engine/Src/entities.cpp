#include "../Include/entities.hpp"

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
	FrameRangeAnimation* newanimation = new FrameRangeAnimation("mushroom", 0, 0, INT_MAX, 0, 0, 1);
	mushroom->SetAnimFilm(film);
	SetAnimator(newanimator);
	newanimator->Start(newanimation, std::time(nullptr));
}

void Squirrel::Do (void) {
	Sprite* squi = GetSprite();
	int *dx = GetDx(), *dy = GetDy();
	int tempdx = *dx;
	squi->GetQuantizer().Move(squi->GetBox(), dx, dy);
	if (*dx == 0) { *dx = tempdx * (-1);}
	squi->Move(*dx, *dy);
	if (GetAnimator()) { return;}
	FrameRangeAnimator* newanimator = new FrameRangeAnimator();
	newanimator->SetOnAction(
		[squi](Animator* animator, const Animation& anim) {
			FrameRange_Action(squi, animator, (const FrameRangeAnimation&) anim);
		}
	);
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("squirrel");
	FrameRangeAnimation* newanimation = new FrameRangeAnimation("squirrel", 0, 0, INT_MAX, 0, 0, 1);
	squi->SetAnimFilm(film);
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
	Sprite* koopa_sprite = GetSprite();
	int *dx = GetDx(), *dy = GetDy();
	int tempdx = *dx;
	koopa_sprite->GetQuantizer().Move(koopa_sprite->GetBox(), dx, dy);
	if (*dx == 0) { tempdx = tempdx*(-1);}
	*dx = tempdx;
	koopa_sprite->Move(*dx, *dy);
	std::string curr;
	std::string prev = koopa_sprite->GetAnimFilm()->GetId();
	int d = 1; bool wakeup = false;
	int start = 0, end = 1, rep = INT_MAX; 
	if (*dx >= 0) 		{ curr = "koopar";}
	if (*dx < 0) 		{ curr = "koopal";}
	if (IsRed()) 		{ curr = "red" + curr;}
	if (IsStunned())	{ 
		curr = "stunned_" + curr; end = 0; d = 2;
		if (*dx == 0) {
			curr = "wakeup_" + curr;
			end = 1; d = 2; rep = 2;
			wakeup = true;
		}
	}

	if (curr.compare(prev) == 0) { return;}
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm(curr);
	FrameRangeAnimation* newanimation = new FrameRangeAnimation(curr, start, end, rep, 0, 0, d);
	FrameRangeAnimator* newanimator = new FrameRangeAnimator();
	newanimator->SetOnAction(
		[koopa_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(koopa_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	if (wakeup) {
		Koopa* koopa = this;
		newanimator->SetOnFinish(
			[koopa](Animator* animator) {
				if (*koopa->GetDx()) { return;}
				koopa->SetDy(-8);
				koopa->SetDx(1);
				koopa->SetStunned(false);
			}
		);
	}
	koopa_sprite->SetAnimFilm(film);
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
			FrameRangeAnimation* f = new FrameRangeAnimation("piran", 0, 14, INT_MAX, 0, -2, 1);
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

void Teleporter::Do () {
	Sprite* s = GetSprite();
	if (GetAnimator()) { return;}
	FrameRangeAnimation* f = new FrameRangeAnimation("tele", 0, 0, INT_MAX, 0, 0, 1);
	((FrameRangeAnimator*)GetAnimator())->Start (
		f,
		std::time(nullptr)
	);
}

void Box::Do () {
	Sprite* s = GetSprite();
	std::string prev = s->GetAnimFilm()->GetId();
	std::string curr;
	int start = 0, end = 2;
	if (GetLives() > 0) { curr = "boxes";}
	else { curr = "dead_boxes"; end = 0;}
	if (prev.compare(curr) == 0) { return;}
	FrameRangeAnimation* f = new FrameRangeAnimation(curr, start, end, INT_MAX, 0, 0, 1);
	FrameRangeAnimator* a = new FrameRangeAnimator();
	a->SetOnAction(
		[s](Animator* animator, const Animation& anim) {
			FrameRange_Action(s, animator, (const FrameRangeAnimation&) anim);
		}
	);
	s->SetAnimFilm(AnimationFilmHolder::Get().GetAnimationFilm(curr));
	SetAnimator(a);
	((FrameRangeAnimator*)GetAnimator())->Start(
		f,
		std::time(nullptr)
	);
}

void Brick::Do () {
}

void Mario::Init (void) {
	Sprite* mario_sprite = new Sprite(
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm("mario_standl"),
		"MARIO"
	);
	SpriteManager::GetSingleton().Add(mario_sprite);
	mario_sprite->PrepareSpriteGravityHandler(tlayer->GetGridLayer(), mario_sprite);
	mario_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), mario_sprite));
	Mario* mario = this;
	mario_sprite->GetGravityHandler().SetOnSolidGround(
		[mario](const Rect& r) {
			int dx = 0, dy = 1;
			mario->GetSprite()->GetQuantizer().Move(r, &dx, &dy);
			return (!dy) ? true : false;
		}
	);
	mario_sprite->GetGravityHandler().SetOnStartFalling(
		[mario]() {
			std::cout << "start falling." << std::endl;
		}
	);
	mario_sprite->GetGravityHandler().SetOnStopFalling(
		[]() {
			std::cout << "stop falling." << std::endl;
		}
	);
	SetSprite(mario_sprite);
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
	std::string str = "koopal";
	if (IsRed()) { str = "red" + str;}
	Sprite* koopa_sprite = new Sprite(
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm(str),
		"KOOPA"
	);
	SpriteManager::GetSingleton().Add(koopa_sprite);

	koopa_sprite->PrepareSpriteGravityHandler(tlayer->GetGridLayer(), koopa_sprite);
	koopa_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), koopa_sprite));
	Koopa* koopa = this;
	koopa_sprite->GetGravityHandler().SetOnSolidGround(
	[koopa_sprite, koopa](const Rect& r) {
		int dx = 0, dy = 1;
		if (*koopa->GetDx() > 0) { dx = 12;}
		else { dx = -12;}
		koopa_sprite->GetQuantizer().Move(r, &dx, &dy);
		return (!dy) ? true : false;
	});
	koopa_sprite->GetGravityHandler().SetOnStartFalling(
	[koopa]() {
		if (!koopa->IsStunned()) {
			koopa->SetDy(-5);
			koopa->SetDx(*koopa->GetDx()*(-1));
		}
		std::cout << "start falling." << std::endl;
	});
	koopa_sprite->GetGravityHandler().SetOnStopFalling(
	[]() {
		std::cout << "stop falling." << std::endl;
	});
	sprite = koopa_sprite;
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm(str);
	FrameRangeAnimation* animation = new FrameRangeAnimation(str, 0, 1, INT_MAX, 0, 0, 1);
	FrameRangeAnimator*	animator = new FrameRangeAnimator();
	animator->SetOnAction(
		[koopa_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(koopa_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	koopa_sprite->SetAnimFilm(film);
	SetAnimator(animator);
	animator->Start(animation, std::time(nullptr));
	SetDx(-1);

	Mario* mario = (Mario*)EntityManager::Get().Get("player");
	CollisionChecker::GetSingleton().Register(
		GetSprite(),
		mario->GetSprite(),
		[mario, koopa] (Sprite* s1, Sprite * s2) {
			if (koopa->GetLives() <= 0) { return;}
			if (mario->IsInv()) { koopa->Damage(); return;}
			Sprite* msprite = mario->GetSprite();
			if (koopa->IsStunned()) {
				if (!msprite->GetGravityHandler().IsFalling()
				&& ((*koopa->GetDx() < 0 && *mario->GetDx() >= 0)
				|| (*koopa->GetDx() > 0 && *mario->GetDx() <= 0)) ) {
					mario->Damage();
					return;
				}
				if (mario->GetSprite()->GetX() > koopa->GetSprite()->GetX()) {
					if (*koopa->GetDx() != 0) {koopa->SetDx(0);}
					else {koopa->SetDx(-1);}
				}
				else if (mario->GetSprite()->GetX() <= koopa->GetSprite()->GetX()){
					if (*koopa->GetDx() != 0) {koopa->SetDx(0);}
					else {koopa->SetDx(1);}
				}
				else {
					koopa->SetDx(0);
				}
				if (msprite->GetGravityHandler().IsFalling()) {
					mario->ResetMass();
					mario->SetDy(-16);
				}
			}
			else {
				if (msprite->GetGravityHandler().IsFalling()) {
					koopa->SetStunned(true);
					mario->ResetMass();
					mario->SetDy(-16);
				}
				else { mario->Damage();}
			}
		}
	);
	auto list = EntityManager::Get().GetAll();
	for (auto it = list.begin(); it != list.end(); ++it) {
		Entity* e = (*it).second;
		Sprite* s = e->GetSprite();
		if (s->GetTypeId().compare("GOOMBA") != 0 
		&& s->GetTypeId().compare("KOOPA") != 0) { continue;}
		CollisionChecker::GetSingleton().Register(
			koopa->GetSprite(),
			s,
			[e, koopa] (Sprite* s1, Sprite* s2) {
				if (koopa->IsStunned() && *koopa->GetDx() != 0) {
					e->Damage();
				}
			}
		);
	}
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
	FrameRangeAnimation* animation = new FrameRangeAnimation("piran", 0, 14, INT_MAX, 0, -2, 1);
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

void Teleporter::Init (void) {
	Sprite* tele_sprite = new Sprite (
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm("tele"),
		"TELE"
	);
	tele_sprite->SetZorder(1);
	SpriteManager::GetSingleton().Add(tele_sprite);
	tele_sprite->SetHasDirectMotion(true);
	tele_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), tele_sprite));
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("tele");
	FrameRangeAnimation* animation = new FrameRangeAnimation("tele", 0, 0, INT_MAX, 0, 0, 1);
	FrameRangeAnimator*	animator = new FrameRangeAnimator();
	animator->SetOnAction(
		[tele_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(tele_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	sprite = tele_sprite;
	tele_sprite->SetAnimFilm(film);
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
			if (!mush->IsDead() && mush->GetSprite()->IsVisible()) {
				al_play_sample(al.power_up_sample,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
				mush->SetDead(true);
				mario->SetSuper(true);
				mario->SetDy(-12);
				SpriteManager::GetSingleton().Remove(mush->GetSprite());
			}
		}
	);
}

void Squirrel::Init () {
	Sprite* squi_sprite  = new Sprite (
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm("squirrel"),
		"SQUIRREL"
	);
	squi_sprite->SetVisibility(false);
	squi_sprite->PrepareSpriteGravityHandler(tlayer->GetGridLayer(), squi_sprite);

	squi_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), squi_sprite));
	squi_sprite->GetGravityHandler().SetOnSolidGround(
	[squi_sprite](const Rect& r) {
		int dx = 0, dy = 1;
		squi_sprite->GetQuantizer().Move(r, &dx, &dy);
		return (!dy) ? true : false;
	});
	squi_sprite->GetGravityHandler().SetOnStartFalling(
	[]() {
		std::cout << "start falling." << std::endl;
	});
	squi_sprite->GetGravityHandler().SetOnStopFalling(
	[]() {
		std::cout << "stop falling." << std::endl;
	});

	SpriteManager::GetSingleton().Add(squi_sprite);

	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("squirrel");
	FrameRangeAnimation* animation = new FrameRangeAnimation("squirrel", 0, 0, INT_MAX, 0, 0, 1);
	FrameRangeAnimator*	animator = new FrameRangeAnimator();
	animator->SetOnAction(
		[squi_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(squi_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	sprite = squi_sprite;
	squi_sprite->SetAnimFilm(film);
	SetAnimator(animator);
	animator->Start(animation, std::time(nullptr));
	
	auto list = EntityManager::Get().GetAll();
	Squirrel* squi = this;
	for (auto it = list.begin(); it != list.end(); ++it) {
		if ((*it).first.substr(0, 3).compare("box") != 0) { continue;}
		Box* box = (Box*)(*it).second;
		CollisionChecker::GetSingleton().Register(
			GetSprite(),
			box->GetSprite(),
			[squi] (Sprite* s1, Sprite* s2) {
				if (*squi->GetDx() == 0) { 
					squi->GetSprite()->SetVisibility(true);
					squi->SetDx(1);
				}
				else { squi->SetDx(*squi->GetDx()*(-1));}
			}
		);
	}
	Mario* mario = (Mario*)EntityManager::Get().Get("player");
	CollisionChecker::GetSingleton().Register(
		GetSprite(),
		mario->GetSprite(),
		[mario, squi] (Sprite* s1, Sprite * s2) {
			if (!squi->IsDead() && squi->GetSprite()->IsVisible()) {
				al_play_sample(al.power_up_sample,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
				squi->SetDead(true);
				mario->SetG(0.01);
				mario->SetDy(-12);
				SpriteManager::GetSingleton().Remove(squi->GetSprite());
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
				if (*one->GetDx() == 0 ) { 
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
			if (!one->IsDead() && one->GetSprite()->IsVisible()) {
				one->SetDead(true);
				mario->SetLives(mario->GetLives()+1);
				mario->AddPoints(1000);
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
	Mario* mario = (Mario*)EntityManager::Get().Get("player");
	CollisionChecker::GetSingleton().Register(
		GetSprite(),
		mario->GetSprite(),
		[mario, star] (Sprite* s1, Sprite* s2) {
			if (star->GetSprite()->IsVisible()) {
				star->GetSprite()->SetVisibility(false);
				mario->SetInv(true);
				FlashAnimation* flash = new FlashAnimation("", 30, 1, 1);
				FlashAnimator* a = new FlashAnimator();
				Sprite* sprite = mario->GetSprite();
				a->SetOnAction(
					[sprite](Animator* animator, const Animation& anim) {
						Sprite_FlashAction(sprite, (FlashAnimator*) animator, &(const FlashAnimation&) anim);
					}
				);
				a->SetOnFinish (
					[mario](Animator* animator) {
						mario->GetSprite()->SetVisibility(true);
						if (((FlashAnimator*)animator)->GetCurrRep() >= 30) {
							mario->SetInv(false);
						}
					}
				);
				SpriteManager::GetSingleton().Remove(star->GetSprite());
				a->Start(flash, std::time(nullptr));
			}
		}
	);
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

	Box* box = this;
	auto list = EntityManager::Get().GetAll();
	for (auto it = list.begin(); it != list.end(); ++it) {
		Entity* e = (*it).second;
		Sprite* s = e->GetSprite();
		if (s->GetTypeId().compare("KOOPA") != 0 &&
			s->GetTypeId().compare("GOOMBA") != 0) { continue;}
		CollisionChecker::GetSingleton().Register(
			box->GetSprite(),
			s,
			[e] (Sprite* s1, Sprite* s2) {
				e->Damage();
			}
		);
	}
}

void Brick::Init () {
	Sprite* brick_sprite  = new Sprite (
		startx,
		starty,
		AnimationFilmHolder::Get().GetAnimationFilm("brick"),
		"BRICK"
	);

	SpriteManager::GetSingleton().Add(brick_sprite);
	brick_sprite->SetHasDirectMotion(true);
	brick_sprite->SetMove(MakeSpriteGridLayerMover (tlayer->GetGridLayer(), brick_sprite));
	AnimationFilm* film = AnimationFilmHolder::Get().GetAnimationFilm("brick");
	FrameRangeAnimation* animation = new FrameRangeAnimation("brick", 0, 0, INT_MAX, 0, 0, 1);
	FrameRangeAnimator*	animator = new FrameRangeAnimator();
	animator->SetOnAction(
		[brick_sprite](Animator* animator, const Animation& anim) {
			FrameRange_Action(brick_sprite, animator, (const FrameRangeAnimation&) anim);
		}
	);
	sprite = brick_sprite;
	brick_sprite->SetAnimFilm(film);
	SetAnimator(animator);
	animator->Start(animation, std::time(nullptr));

	Brick* brick = this;
	Mario* mario = (Mario*)EntityManager::Get().Get("player");
	CollisionChecker::GetSingleton().Register(
		brick->GetSprite(),
		mario->GetSprite(),
		[mario, brick] (Sprite* s1, Sprite* s2) {
			if (mario->IsSuper() || mario->IsInv()) {
				brick->Damage();
			}
		}
	);
}