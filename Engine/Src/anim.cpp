#include "../Include/anim.hpp"

AnimationFilmHolder 	AnimationFilmHolder::holder;
AnimatorManager 		AnimatorManager::singleton;
SystemClock				SystemClock::singleton;
SpriteManager			SpriteManager::singleton;
CollisionChecker		CollisionChecker::singleton;

AnimationFilm::AnimationFilm (Bitmap _bitmap, const std::vector<Rect>& _boxes, const std::string& _id) {
	SetBitmap(_bitmap);
	boxes = _boxes;
	id = _id;
}

int AnimationFilmHolder::ParseEntry (
int startPos,
std::ifstream& f,
std::string& id,
std::string& path,
std::vector<Rect>& rects) {
	int c, x, y, w, h;
	if (f >> c >> id >> path) {
		std::cout << id << " " << path;
		while (c--) {
			f >> x >> y >> w >> h;
			std::cout << ", " << x << " " << y << " " << w << " " << h;
			rects.push_back({x, y, w, h});
		}
		std::cout << std::endl;
		return 1;
	}
	return 0;
}

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
	lastTime += offset;
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

void LatelyDestroyable::Delete (void) {
	assert(!dying); dying = true; delete this;
}

void DestructionManager::Register (LatelyDestroyable* d) {
	assert(!d->IsAlive());
	dead.push_back(d);
}

uint64_t SystemClock::milli_secs (void) const {
	return std::chrono::duration_cast<std::chrono::milliseconds> (clock.now().time_since_epoch()).count();
}
uint64_t SystemClock::micro_secs (void) const {
	return std::chrono::duration_cast<std::chrono::microseconds>(clock.now().time_since_epoch()).count();
}
uint64_t SystemClock::nano_secs (void) const {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(clock.now().time_since_epoch()).count();
}
uint64_t GetSystemTime (void) {
	return SystemClock::Get().milli_secs();
}

void TickAnimator::Progress (timestamp_t currTime) {
	if (!anim->IsDiscrete()) {
		elapsedTime = currTime - lastTime;
		lastTime = currTime;
		NotifyAction(*anim);
	}
	else
	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay())   {
		lastTime += anim->GetDelay();
		NotifyAction(*anim);
		if (!anim->IsForever() && ++currRep == anim->GetReps()) {
			state = ANIMATOR_RUNNING;
			NotifyStopped();
			return;
		}
	}
}

void ScrollAnimator::Progress (timestamp_t currTime) {
	lastTime = currTime;
	NotifyAction(*anim);
	if (++currRep >= anim->GetScroll().size()) {
		state = ANIMATOR_FINISHED;
		NotifyStopped ();
		return;
	}
}
void Sprite_ScrollAction (Sprite* sprite, const ScrollAnimator* animator, const ScrollAnimation* anim) {
	int rep = animator->GetCurrRep();
	sprite->Move(anim->GetDx(rep), anim->GetDy(rep));
}
/*animator->SetOnAction(
	[sprite](Animator* animator, const Animation& anim) {
		Sprite_ScrollAction(sprite, (ScrollAnimator*) animator, (const ScrollAnimation&) anim);
	}
);*/
void FlashAnimator::Progress (timestamp_t currTime) {
	while (currTime > lastTime && (currTime - lastTime) >= anim->GetHideDelay())   {
		lastTime += anim->GetHideDelay();
		NotifyAction(*anim);
		if (++currRep == anim->GetRepetitions()) {
			state = ANIMATOR_RUNNING;
			NotifyStopped();
			return;
		}
	}
}
void Sprite_FlashAction (Sprite* sprite, const FlashAnimator* animator, const FlashAnimation* anim) {
	if (animator->GetCurrRep()%2 == 0) { sprite->SetVisibility(false);}
	else { sprite->SetVisibility(true);}
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

template <class T> bool clip_rect (
	T x, 		T y, 		T w, 		T h,
	T wx,		T wy,		T ww,		T wh,
	T* cx,	T* cy,	T* cw,	T* ch
) {
	*cw = T(std::min(wx + ww, x + w)) - (*cx = T(std::max(wx, x)));
	*ch = T(std::min(wy + wh, y + h)) - (*cy = T(std::max(wy, y)));
	return *cw > 0 && *ch > 0;
}

bool clip_rect (const Rect& r, const Rect& area, Rect* result) {
	return clip_rect (
		r.x,
		r.y,
		r.w,
		r.h,
		area.x,
		area.y,
		area.w,
		area.h,
		&result->x,
		&result->y,
		&result->w,
		&result->h
	);
}

bool Clipper::Clip (const Rect& r, const Rect& dpyArea, Point* dpyPos, Rect* clippedBox) const {
	Rect visibleArea;
	if (!clip_rect(r, viewRect, &visibleArea)) 
		{ clippedBox->w = clippedBox->h = 0; return false; }
	else {
		clippedBox->x = r.x - visibleArea.x;
		clippedBox->y = r.y - visibleArea.y;

		clippedBox->w = visibleArea.w;
		clippedBox->h = visibleArea.h;

		dpyPos->x = dpyArea.x + (visibleArea.x - viewRect.x);
		dpyPos->y = dpyArea.y + (visibleArea.y - viewRect.y);

		return true;
	}
}

const Clipper& MakeTileLayerClipper (TileLayer* layer) {
	Clipper* c = new Clipper();
	return c->SetView(layer->GetViewWindow());
}

const Sprite::Mover MakeSpriteGridLayerMover (GridLayer* gridLayer, Sprite* sprite) {
	return [gridLayer, sprite](const Rect& r, int* dx, int* dy) {
		gridLayer->FilterGridMotion(sprite->GetBox(), dx, dy);
	};
}
void Sprite::Display (Bitmap dest, const Rect& dpyArea, const Clipper& clipper) const {
	Rect	clippedBox;
	Point dpyPos;
	Rect clippedFrame;
	if( clipper.Clip(GetBox(), dpyArea, &dpyPos, &clippedBox)) {
		Rect clippedFrame{
			frameBox.x + clippedBox.x,
			frameBox.y + clippedBox.y,
			clippedBox.w,
			clippedBox.h
		};
		MaskedBlit(
			currFilm->GetBitmap(),
			clippedFrame,
			dest,
			dpyPos
		);
	}
}

bool Sprite::CollisionCheck (const Sprite* s) const {
	Rect r1 = GetBox(), r2 = s->GetBox();
	if ( r1.x < r2.x + r2.w &&
			r1.x + r1.w > r2.x &&
			r1.y < r2.y + r2.h &&
			r1.y + r1.h > r2.y) { return true;}
	return false;
}

void GravityHandler::Check (const Rect& r) {
	if (gravityAddicted) {
		if (onSolidGround(r)) {
			if (isFalling) {
				isFalling = false;
				onStopFalling();
			}
		}
		else if (!isFalling) {
			isFalling = true;
			onStartFalling();
		}
	}
}

void CollisionChecker::Cancel (Sprite* s1, Sprite* s2) {
	auto i = std::find_if(
		entries.begin(),
		entries.end(),
		[s1, s2](const Entry& e) {
			return 	std::get<0>(e) 	== s1 && std::get<1>(e) == s2 ||
						std::get<0>(e) 	== s2 && std::get<1>(e) == s1;
		}
	);
}

void SpriteManager::Add (Sprite* s) {
	dpyList.push_back(s);
	dpyList.sort(
		[](Sprite* first, Sprite* sec) {
			return (first->GetZorder() < sec->GetZorder());
		}
	);
	for (auto it = dpyList.cbegin(); it != dpyList.cend(); ++it) {
		types[(*it)->GetTypeId()].push_back((*it));
	}
}

void SpriteManager::Remove (Sprite* s) {
	std::string typid = s->GetTypeId();
	types[typid].remove(s);
	dpyList.remove(s);
}

void DisplaySprites (Bitmap dest, const Rect& dpyArea, TileLayer* tlayer) {
	auto dpyList = SpriteManager::GetSingleton().GetDisplayList();
	for (auto it = dpyList.begin(); it != dpyList.end(); ++it) {
		if (!(*it)->IsVisible()) { continue;}
		const Clipper& clipper = MakeTileLayerClipper(tlayer);
		(*it)->Display(dest, dpyArea, clipper);
	}
}

void CollisionChecker::Check (void) const {
	for (auto& e : entries) {
		if (std::get<0>(e)->CollisionCheck(std::get<1>(e))) {
			std::get<2>(e)( std::get<0>(e), std::get<1>(e));
		}
	}
}