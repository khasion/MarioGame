#ifndef ANIM_HPP
#define ANIM_HPP
#include "layer.hpp"

class AnimationFilm {
	std::vector<Rect> 	boxes;
	Bitmap					bitmap = nullptr;
	std::string				id;
public:
	byte 		GetTotalFrames (void) const 	{ return boxes.size();}
	Bitmap	GetBitmap (void) const 			{ return bitmap;}
	auto		GetId (void) const -> const std::string& { return id;}
	const Rect& GetFrameBox (byte frameNo) const 
						{ assert(boxes.size()>frameNo); return boxes[frameNo];}
	void DisplayFrame (Bitmap dest, const Point& at, byte frameNo) const {
		MaskedBlit (bitmap, GetFrameBox(frameNo), dest, at);
	}
	void SetBitmap (Bitmap b) {
		assert(b); bitmap = b;
	}
	void Append (const Rect& r) { boxes.push_back(r);}
	AnimationFilm (Bitmap, const std::vector<Rect>&, const std::string&);
};

class AnimationFilmHolder {
public:
	using Films = std::map<std::string, AnimationFilm*>;
private:
	Films films;
	BitmapLoader bitmaps;// only for loading of film bitmaps 
	static AnimationFilmHolder holder;// singleton
	AnimationFilmHolder (void) {}
	~AnimationFilmHolder() {bitmaps.CleanUp();}
public:
	static AnimationFilmHolder& Get (void) {
		return holder;
	}
	static void Destroy (void) {
		delete &holder;
	}
		// TODO(4u): set a parsing functor implemented externally to the class
	static int ParseEntry ( // -1=error, 0=ended gracefully, else #chars read
			int startPos,
			std::ifstream& text,
			std::string& id,
			std::string& path,
			std::vector<Rect>& rects
	);
	void LoadAll (const std::string& path) {
		int pos = 0;
		std::ifstream f (path);
		while (true) {
			std::string id, path;
			std::vector<Rect> rects;
			auto i = ParseEntry(pos, f, id, path, rects);
			assert(i >= 0);
			if (!i) {break;}
			pos += i;
			assert(!films[id]);
			films[id] = new AnimationFilm(bitmaps.Load(path), rects, id);
		}
	}
	AnimationFilm* GetAnimationFilm (std::string id) {
		return films[id];
	}
};

class Animation {
protected:
	std::string id;
public:
	const std::string&	GetId (void) { return id;}
	void						SetId (const std::string& _id) { id = _id;}
	virtual Animation*	Clone (void) const = 0;
	Animation (const std::string& _id) : id(_id){}
	virtual ~Animation() {}
};

class MovingAnimation : public Animation {
protected:
	unsigned	reps = 1; // 0=forever
	int		dx = 0, dy = 0;
	unsigned	delay = 0;
public:
	using Me = MovingAnimation;
	int			GetDx (void) const	{ return dx;}
	Me&			SetDx (int v)			{ dx = v; return *this;}
	int			GetDy (void) const	{ return dy;}
	Me&			SetDy (int v)			{ dy = v; return *this;}
	unsigned		GetDelay(void) const	{ return delay;}
	Me&			SetDelay (unsigned v){ delay = v; return *this;}
	unsigned		GetReps (void) const { return reps;}
	Me&			SetReps (unsigned n) { reps = n; return *this;}
	bool			IsForever (void)		{ return !reps;}
	Me&			SetForever (void)		{ reps = 0; return *this;}
	Animation*	Clone (void) const override
						{ return new MovingAnimation(id, reps, dx, dy, delay);}
	MovingAnimation (
			const std::string& _id, unsigned _reps, int _dx, int _dy, unsigned _delay
	): Animation(_id), reps (_reps), dx(_dx), dy(_dy), delay(_delay) {}
};

class FrameRangeAnimation : public MovingAnimation {
protected:
	unsigned start = 0, end = 0;
public:
	using Me = FrameRangeAnimation;
	unsigned		GetStartFrame (void) const { return start;}
	Me&			SetStartFrame (unsigned v)	{ start = v; return *this;}
	unsigned		GetEndFrame (void) const	{ return end;}
	Me&			SetEndFrame (unsigned v)	{ end = v; return *this;}
	Animation*	Clone (void) const override {
		return new FrameRangeAnimation (id, start, end, GetReps(), GetDx(), GetDy(), GetDelay());
	}
	FrameRangeAnimation (
		const std::string& _id,
		unsigned s, unsigned e,
		unsigned r, int dx, int dy, int d
	): start(s), end(e), MovingAnimation(_id, r, dx, dy, d) {}
};

class FrameListAnimation : public MovingAnimation {
public:
	using Frames = std::vector<unsigned>;
protected:
	Frames	frames;
public:
	const Frames& 	GetFrames (void) const { return frames;}
	void				SetFrames (const Frames& f) { frames = f;}
	Animation* Clone (void) const override {
		return new FrameListAnimation (
			id, frames, GetReps(), GetDx(), GetDy(), GetDelay()
		);
	}
	FrameListAnimation (
		const std::string& _id,
		const Frames& _frames,
		unsigned r, int dx, int dy, unsigned d
	): frames(_frames), MovingAnimation(_id, r, dx, dy, d) {}
};

struct PathEntry {
	int		dx = 0, dy = 0;
	unsigned frame = 0;
	unsigned delay = 0;
	PathEntry (void)=default;
	PathEntry (const PathEntry&)=default;
};

class MovingPathAnimation : public Animation {
public:
	using Path = std::vector<PathEntry>;
private:
	Path path;
public:
	const Path&	GetPath (void) const 	{ return path;}
	void			SetPath (const Path& p)	{ path = p;}
	Animation*	Clone (void) const override {
		return new MovingPathAnimation (id, path);
	}
	MovingPathAnimation (const std::string& _id, const Path& _path) :
		path(_path), Animation(id) {}
};

class FlashAnimation : public Animation {
private:
	unsigned repetitions = 0;
	unsigned	hideDelay = 0;
	unsigned	showDelay = 0;
public:
	using Me= FlashAnimation;
	Me&			SetRepetitions(unsigned n) { repetitions = n; return*this; }
	unsigned 	GetRepetitions(void) const { return repetitions; }
	Me& 			SetHideDelay(unsigned d) { hideDelay = d; return*this; }
	unsigned		GetHideDelay(void) const{ return hideDelay; }
	Me&			SetShowDeay(unsigned d) { showDelay= d; return*this; }
	unsigned		GetShowDeay(void) const{ return showDelay; }
	Animation* 	Clone (void) const override{
		return new FlashAnimation(id, repetitions, hideDelay, showDelay); 
	}
	FlashAnimation (const std::string& _id, unsigned n, unsigned show, unsigned hide) :
		Animation(id), repetitions(n), hideDelay(hide), showDelay(show) {}
};

struct ScrollEntry {
	int			dx = 0;
	int 			dy = 0;
	unsigned		delay = 0;
};

class ScrollAnimation : public Animation {
public:
	using Scroll = std::vector<ScrollEntry>;
private:
	Scroll scroll;
public:
	const Scroll& 	GetScroll (void) const 		{ return scroll;}
	void				SetScroll (const Scroll& p){ scroll = p;}
	Animation*		Clone (void) const override{
		return new ScrollAnimation(id, scroll); 
	}
	ScrollAnimation(const std::string& _id, const Scroll& _scroll) :
		Animation(_id), scroll(_scroll) {}
};

typedef uint64_t timestamp_t;

enum animatorstate_t {
	ANIMATOR_FINISHED, ANIMATOR_RUNNING, ANIMATOR_STOPPED
};

class Animator {	
public:
	using OnFinish = std::function<void(Animator*)>;
	using OnStart 	= std::function<void(Animator*)>;
	using OnAction = std::function<void(Animator*, const Animation&)>;
protected:
	timestamp_t 		lastTime = 0;
	animatorstate_t 	state = ANIMATOR_FINISHED;
	OnFinish				onFinish;
	OnStart				onStart;
	OnAction				onAction;
	void					NotifyStopped(void);
	void					NotifyStarted(void);
	void					NotifyAction(const Animation&);
	void					Finish (bool isForced = false);
public:
	void									Stop (void);
	bool									HasFinished(void) const 			{ return state != ANIMATOR_RUNNING; }
	virtual void 						TimeShift(timestamp_t offset);
	virtual void 						Progress (timestamp_t currTime) = 0;
	template<typename Tfunc> void SetOnFinish(const Tfunc& f) 		{ onFinish = f; }
	template<typename Tfunc> void SetOnStart(const Tfunc& f) 		{ onStart= f; }
	template<typename Tfunc> void SetOnAction(const Tfunc& f) 		{ onAction= f; }

	Animator (void);
	Animator (const Animator&) = delete;
	Animator (Animator&&) = delete;
	virtual ~Animator ();
};

class MovingAnimator : public Animator {
protected:
	MovingAnimation* 	anim = nullptr;
	unsigned				currRep = 0;
public:
	void Progress (timestamp_t currTime);
	auto GetAnim (void) const -> const MovingAnimation& { return *anim;}
	void Start (MovingAnimation* a, timestamp_t t) {
		anim 		= a;
		lastTime = t;
		state		= ANIMATOR_RUNNING;
		currRep	= 0;
		NotifyStarted();
	}
	MovingAnimator (void) = default;
};

class FrameRangeAnimator : public Animator {
protected:
	FrameRangeAnimation*	anim = nullptr;
	unsigned currFrame = 0;
	unsigned currRep = 0;
public:
	void		Progress (timestamp_t currTime);
	unsigned	GetCurrFrame (void) const { return currFrame;}
	unsigned	GetCurrRep (void) const		{ return currRep;}
	void		Start (FrameRangeAnimation* a, timestamp_t t) {
		anim 		= a;
		lastTime = t;
		state		= ANIMATOR_RUNNING;
		currFrame= anim->GetStartFrame();
		currRep	= 0;
		NotifyStarted();
		NotifyAction(*anim);
	}
	FrameRangeAnimation* GetAnim (void) { return anim;}
	FrameRangeAnimator (void) = default;
};

class AnimatorManager {	
private:
	std::set<Animator*> running, suspended;
	static AnimatorManager singleton;
	AnimatorManager (void) = default;
	AnimatorManager (const AnimatorManager&) = delete;
	AnimatorManager (AnimatorManager&&) = delete;
public:
	void Register (Animator* a)
	{ assert(a->HasFinished()); suspended.insert(a);}
	void Cancel (Animator* a)
	{ assert(a->HasFinished()); suspended.erase(a);}
	void MarkAsRunning (Animator* a)
	{ assert(!a->HasFinished()); suspended.erase(a); running.insert(a);}
	void MarkAsSuspended (Animator* a)
	{ assert(a->HasFinished()); running.erase(a); suspended.insert(a);}
	void TimeShift (unsigned dt) {
		for (auto* a : running) {
			a->TimeShift(dt);
		}
	}
	void Progress (timestamp_t currTime) {
		auto copied (running);
		for (auto* a : copied) {
			a->Progress(currTime);
		}
	}
	static auto GetSingleton (void) -> AnimatorManager& { return singleton;}
	static auto GetSingletonConst (void) -> const AnimatorManager& { return singleton;}
};

class LatelyDestroyable;
class DestructionManager {
	std::list<LatelyDestroyable*> dead;
	static DestructionManager singleton;
public:
	void Register (LatelyDestroyable* d);
	void Commit (void);
	static auto Get (void) -> DestructionManager& { return singleton; }
};
class LatelyDestroyable {
protected:
	friend class DestructionManager;
	bool alive = true;
	bool dying = false;
	virtual ~LatelyDestroyable() { assert(dying);}
	void Delete (void);
public:
	bool IsAlive (void) const { return alive;}
	void Destroy (void) {
		if (alive) {
			alive = false;
			DestructionManager::Get().Register(this);
		}
	}
	LatelyDestroyable (void) = default;
};

#include <chrono>

class SystemClock final {
private:
	std::chrono::high_resolution_clock clock;
	static SystemClock singleton;
public:
	static auto Get (void) -> SystemClock& { return singleton;}
	uint64_t		milli_secs 	(void) const;
	uint64_t		micro_secs 	(void) const;
	uint64_t		nano_secs 	(void) const;
};

class TickAnimation : public Animation {
protected:
	unsigned delay = 0;
	unsigned reps = 1;
	bool		isDiscrete = true;
	bool 		Inv (void) const { return isDiscrete || reps == 1;}
public:
	using Me = TickAnimation;
	unsigned 	GetDelay (void) const			{ return delay;}
	Me&			SetDelay (unsigned v) 			{ delay = v; return*this; }
	unsigned		GetReps(void) const				{ return reps; }
	Me&			SetReps(unsigned n)				{ reps = n;  return*this; }
	bool			IsForever(void) const			{ return !reps; }
	Me&			SetForever(void) 					{ reps = 0; return*this; }
	bool			IsDiscrete(void) const			{ return isDiscrete; }
	Animation*	Clone (void) const override	{ return new TickAnimation(id, delay, reps, isDiscrete); }

	TickAnimation (const std::string& _id, unsigned d, unsigned r, bool discrete) :
		Animation(_id), delay(d), reps(r), isDiscrete(discrete) { assert(Inv());}
};

class TickAnimator : public Animator {
protected:
	TickAnimation* anim = nullptr;
	unsigned			currRep = 0;
	unsigned			elapsedTime = 0;
public:
	void		Progress (timestamp_t currTime) override;
	unsigned	GetCurrRep (void) const { return currRep;}
	unsigned GetElapsedTime (void) const { return elapsedTime;}
	float		GetElapsedTimeNormalised (void) const
					{ return float(elapsedTime) / (float)(anim->GetDelay());}
	void 		Start (TickAnimation& a, timestamp_t t) {
		anim			= 	(TickAnimation*) a.Clone();
		lastTime		=	t;
		state			= ANIMATOR_RUNNING;
		currRep		= 0;
		elapsedTime	= 0;
		NotifyStarted();
	}
	TickAnimator (void) = default;
};

template <typename Tnum>
int number_sign (Tnum x) { return x > 0 ? 1 : x < 0 ? -1 : 0;}

class MotionQuantizer {
public:
	using Mover = std::function<void(const Rect& r, int* dx, int* dy)>;
protected:
	int 	horizMax = 0, vertMax = 0;
	Mover mover;
	bool	used = false;
public:
	MotionQuantizer&	SetRange (int h, int v)
								{ horizMax = h, vertMax = v; used = true; return *this;}
	template <typename Tfunc>
	MotionQuantizer& 	SetMover (const Tfunc& f)
								{mover = f; return *this;}
	void					Move (const Rect& r, int* dx, int* dy);
	MotionQuantizer (void) = default;
	MotionQuantizer (const MotionQuantizer&) = default;
};

template <class T> bool clip_rect (
	T x, 		T y, 		T w, 		T h,
	T wx,		T wy,		T ww,		T wh,
	T* cx,	T* cy,	T* cw,	T* ch
);
bool clip_rect (const Rect& r, const Rect& area, Rect* result);

class Clipper {
private:
	Rect viewRect;
public:
	Clipper&	SetView (const Rect& r) { viewRect = r; return *this;}
	bool		Clip (const Rect& r, const Rect& dpyArea, Point* dpyPos, Rect* clippedBox) const;

	Clipper (void) = default;
	Clipper (const Clipper&) = default;
};

class GravityHandler {
public:
	using OnSolidGroundPred	= std::function<bool(const Rect&)>;
	using OnStartFalling		= std::function<void(void)>;
	using OnStopFalling		= std::function<void(void)>;
protected:
	bool						gravityAddicted	= true;
	bool						isFalling			= false;
	OnSolidGroundPred		onSolidGround;
	OnStartFalling			onStartFalling;
	OnStopFalling			onStopFalling;
public:
	template <typename T> 	void SetOnStartFalling (const T& f)
										{ onStartFalling = f;}
	template <typename T>	void SetOnStopFalling (const T& f)
										{ onStopFalling = f;}
	template <typename T>	void SetOnSolidGround (const T& f)
										{ onSolidGround = f;}
	void Reset (void) { isFalling = false;}
	void Check (const Rect& r);
	bool IsFalling (void) { return isFalling;}
};

class Sprite {
public:
	using Mover = std::function<void(const Rect&, int* dx, int* dy)>;
protected:
	byte					frameNo = 0;
	Rect					frameBox;
	int 					x = 0, y = 0;
	bool					isVisible = false;
	AnimationFilm*		currFilm = nullptr;
	Clipper*				boundingArea = nullptr;
	unsigned				zorder = 0;
	std::string			typeId, stateId;
	Mover					mover;
	MotionQuantizer	quantizer;
	bool					directMotion = false;
	GravityHandler		gravity;
public:
	template <typename Tfunc>
	void 			SetMove (const Tfunc& f)
						{ quantizer.SetMover(mover = f);}
	const Rect	GetBox (void) const
						{ return {x, y, frameBox.w, frameBox.h};}
	void 			Move (int dx, int dy) {
						if (directMotion) {
							x += dx; y += dy;
						}
						else {
							quantizer.Move(GetBox(), &dx, &dy);
							gravity.Check(GetBox());
							x += dx; y += dy;
						}
					}
	void			SetPos (int _x, int _y) { x = _x; y = _y;}
	void			SetZorder (unsigned z) 	{ zorder = z;}
	unsigned		GetZorder (void)			{ return zorder;}

	AnimationFilm* GetAnimFilm (void)					{ return currFilm;}
	void				SetAnimFilm (AnimationFilm* film)
							{ currFilm = film; frameNo = currFilm->GetTotalFrames(); SetFrame(0); }

	MotionQuantizer& GetQuantizer (void) { return quantizer;}

	void	SetFrame (byte i) {
		if (i != frameNo) {
			assert(i < currFilm->GetTotalFrames());
			frameBox = currFilm->GetFrameBox(frameNo = i);
		}
	}
	byte	GetFrame (void) const { return frameNo;}
	void	SetBoundingArea (Clipper& area)
				{ assert(!boundingArea); boundingArea = &area;}
	void	SetBoundingArea (Clipper* area)
				{ assert(!boundingArea); boundingArea = area;}
	auto	GetBoundingArea (void)	 const -> const Clipper*
				{ return boundingArea;}
	auto	GetTypeId(void) -> const std::string& { return typeId; }
	void	SetVisibility(bool v) { isVisible= v; }
	bool	IsVisible(void) const{ return isVisible; }
	bool 	CollisionCheck(const Sprite* s) const;

	GravityHandler&	GetGravityHandler (void)
								{ return gravity;}
	void 					SetHasDirectMotion (bool v)		{ directMotion = v;}
	bool					GetHasDirectMotion (void) const 	{ return directMotion;}

	void PrepareSpriteGravityHandler (GridLayer* gridlayer, Sprite* sprite) {
		sprite->GetGravityHandler().SetOnSolidGround(
			[gridlayer](const Rect& r)
				{ return gridlayer->IsOnSolidGround(r);}
		);
	}

	void	Display (Bitmap dest, const Rect& dpyArea, const Clipper& clipper) const;

	Sprite (int _x, int _y, AnimationFilm* film, const std::string& _typeId= ""):
		x(_x), y(_y), currFilm(film), typeId (_typeId)
		{ frameNo = currFilm->GetTotalFrames(); SetFrame(0); }
};

class CollisionChecker final {
public:
	using 	Action = std::function<void(Sprite* s1, Sprite* s2)>;
	static	CollisionChecker singleton;
protected:
	using Entry = std::tuple<Sprite*, Sprite*, Action>;
	std::list<Entry> entries;
public:
	template <typename T>
	void Register (Sprite* s1, Sprite* s2, const T& f) {
		entries.push_back(std::make_tuple(s1, s2, f));
	}
	void Cancel (Sprite* s1, Sprite* s2);
	void Check	(void) const;

	static auto GetSingleton (void) -> CollisionChecker&
						{ return singleton;}
	static auto GetSingletonConst (void) -> const CollisionChecker&
						{ return singleton;}
};

class SpriteManager final {
public:
	using SpriteList	= std::list<Sprite*>;
	using TypeLists	= std::map<std::string, SpriteList>;
private:
	SpriteList	dpyList;
	TypeLists	types;
	static SpriteManager	singleton;
public:
	void			Add (Sprite* s);
	void			Remove (Sprite* s);
	auto			GetDisplayList (void) -> const SpriteList&
						{ return dpyList;}
	auto 			GetTypeList (const std::string& typeId)
						{ return types[typeId];}
	static auto GetSingleton (void) -> SpriteManager&
						{ return singleton;}
	static auto GetSingletonConst (void) -> const SpriteManager&
						{ return singleton;}
};

void FrameRange_Action (Sprite* sprite, Animator* animator, const FrameRangeAnimation& anim);

const Sprite::Mover MakeSpriteGridLayerMover (GridLayer*, Sprite*);
void Sprite_MoveAction (Sprite* sprite, const MovingAnimation& anim);
void DisplaySprites (Bitmap, const Rect&, TileLayer*);

#endif