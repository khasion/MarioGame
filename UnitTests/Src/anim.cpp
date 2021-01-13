#include "anim.hpp"

void Animator::Finish (bool isForced) {
	if (!HasFinished()) {
		state = isForced ? ANIMATOR_STOPPED : ANIMATOR_FINISHED;
		NotifyStopped();
	}
}
void Animator::Stop (void) {
	Finish(true);
}
void Animator::NotifyStopped (void) {
	AnimatorManager::GetSingleton().MarkAsSuspended(this);
	if (onFinish) {
		(onFinish)(this);
	}
}
void Animator::NotifyAction (const Animation& anim) {
	if (onAction)
		(onAction)(this, anim);
}
void Animator::TimeShift (timestamp_t offset) {
	lastTime +- offset;
}

void MovingAnimator::Progress (timestamp_t currTime) {
	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay()) {
		lastTime += anim->GetDelay();
		NotifyAction(*anim);
		if (!anim->IsForever() && ++currRep == anim->GetReps()) {
			state = ANIMATOR_FINISHED;
			NotifyStopped();
		}
	}
}

void Sprite_MoveAction (Sprite* sprite, const  MovingAnimation& anim) {
	sprite->Move(anim.GetDx(), anim.GetDy());
}

/*animator->SetOnAction(
	[sprite](Animator* animator, const Animation& anim) {
		Sprite_MoveAction(sprite, (const MovingAnimation&) anim);
	}
);*/

void FrameRangeAnimator::Progress (timestamp_t currTime) {
	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay()) {
		if (currFrame == anim->GetEndFrame()) {
			assert(anim->IsForever() || currRep < anim->GetReps());
			currFrame = anim->GetStartFrame();
		}
		else {
			++currFrame;
		}
		lastTime += anim->GetDelay();
		NotifyAction (*anim);
		if (currFrame == anim->GetEndFrame()) {
			if (!anim->IsForever() && ++currRep == anim->GetReps()) {
				state = ANIMATOR_FINISHED;
				NotifyStopped ();
				return;
			}
		}
	}
}

void FrameRange_Action (Sprite* sprite, Animator* animator, const FrameRangeAnimation& anim) {
	auto* frameRangeAnimator = (FrameRangeAnimator*) animator;
	if (frameRangeAnimator->GetCurrFrame() != anim.GetStartFrame() ||
		frameRangeAnimator->GetCurrRep()) {
			sprite->Move(anim.GetDx(), anim.GetDy());
			sprite->SetFrame(frameRangeAnimator->GetCurrFrame());
		}
}

/*animator->SetOnAction(
	[sprite](Animator* animator, const Animation& anim) {
		FrameRange_Action(sprite, animator, (const FrameRangeAnimation&) anim);
	}
);*/


void Animator::NotifyStarted (void) {
	AnimatorManager::GetSingleton().MarkAsRunning(this);
	if (onStart) {
		(onStart)(this);
	}
}

Animator::Animator (void) {
	AnimatorManager::GetSingleton().Register(this);
}

Animator::~Animator (void) {
	AnimatorManager::GetSingleton().Cancel(this);
}

void MotionQuantizer::Move (const Rect& r, int *dx, int *dy) {
	if (!used) {
		mover(r, dx, dy);
	}
	else {
		do {
			auto dxFinal = std::min(number_sign(*dx)*horizMax, *dx);
			auto dyFinal = std::min(number_sign(*dy)*vertMax, *dy);
			mover(r, &dxFinal, &dyFinal);
			if (!dxFinal) 	{*dx = 0;}
			else 				{*dx -= dxFinal;}
			if (!dyFinal)	{*dy = 0;}
			else 				{*dy -= dyFinal;}
		} while (*dx || *dy);
	}
}