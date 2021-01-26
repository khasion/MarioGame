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
void Coin::Do (Sprite* mario) {
	Sprite* s = GetSprite();
	Sprite* m = mario;
	if (s->GetX() - 16 <= m->GetX()
	&& s->GetX() + s->GetWidth() + 16 >= m->GetX()
	&& s->GetFrame() >= s->GetAnimFilm()->GetTotalFrames()-2) { 
		GetAnimator()->Stop();
	}
	else {
		if (GetAnimator()->HasFinished()) {
			 //FrameRangeAnimation (id, start, end, GetReps(), GetDx(), GetDy(), GetDelay());
			FrameRangeAnimation* f = new FrameRangeAnimation("coin", 0, 2, INT_MAX, 0, 0, 1);
			((FrameRangeAnimator*)GetAnimator())->Start(
				f,
				std::time(nullptr)
			);
			s->SetPos(s->GetX(),position_y);
		}
	}
	if (s->GetFrame() == 0) {
		s->SetPos(s->GetX(),position_y);
	}
}