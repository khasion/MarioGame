#ifndef ANIM_HPP
#define ANIM_HPP
#include "layer.hpp"

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
	Animation*	Clone (void) const override{
		return new FrameRangeAnimation (id, start, end, GetReps(), GetDx(), GetDy(), GetDelay());
	}
	FrameRangeAnimation (
		const std::string& _id,
		unsigned s, unsigned e,
		unsigned r, int dx, int dy, int d
	): start(s), end(e), MovingAnimation(id, r, dx, dy, d) {}
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
	): frames(_frames), MovingAnimation(id, r, dx, dy, d) {}
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
};

#endif