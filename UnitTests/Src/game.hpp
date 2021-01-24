#ifndef GAME_HPP
#define GAME_HPP
#include "anim.hpp"

#define DISP_W 640
#define DISP_H 480
#define MAX_FPS 60 
#define FPS 1.0 / MAX_FPS

#define TILE_MAP 	"../../ThirdParty/action.csv"
#define BG_MAP 	"../../ThirdParty/background.csv"
#define CHAR_MAP 	"../../ThirdParty/characters.gif"
#define ANIM_PATH	"anim.conf"

#define KEY_SEEN 1
#define KEY_RELEASED 2


class Game {
	public:
		using Action 	= std::function<void(void)>;
		using Pred		= std::function<bool(void)>;
	private:
		Action render, anim, input, ai, physics, destruct, collisions, user, pauseResume;
		Pred 	done;
		bool 	isPaused = false;
		uint64_t pauseTime = 0;
		void 	Invoke (const Action& f) { if (f) f();}
	public:
		// Setters
		template <typename Tfunc>
		void setRender (const Tfunc& f) { render = f;}
		template <typename Tfunc>
		void setAnim (const Tfunc& f) { anim = f;}
		template <typename Tfunc>
		void setInput (const Tfunc& f) { input = f;}
		template <typename Tfunc>
		void setAI (const Tfunc& f) { ai = f;}
		template <typename Tfunc>
		void setPhysics (const Tfunc& f) { physics = f;}
		template <typename Tfunc>
		void setDestruct (const Tfunc& f) { destruct = f;}
		template <typename Tfunc>
		void setCollisions (const Tfunc& f) { collisions = f;}
		template <typename Tfunc>
		void setUser (const Tfunc& f) { user = f;}
		template <typename Tfunc>
		void setDone (const Tfunc& f) { done = f;}
		void SetOnPauseResume (const Action& f) { pauseResume = f;}
		//
		void Render (void) { Invoke(render);}
		void ProgressAnimations (void) { Invoke(anim);}
		void Input (void) { Invoke(input);}
		void AI (void) { Invoke(ai);}
		void Physics (void) { Invoke(physics);}
		void CollisionChecking (void) { Invoke(collisions);}
		void CommitDestructions (void) { Invoke(destruct);}
		void UserCode (void) { Invoke(user);}
		bool IsFinished (void) { return done();}
		void MainLoop (void);
		void MainLoopIteration (void);

		void Pause (uint64_t t) { isPaused = true; Invoke(pauseResume); pauseTime = t;}
		void Resume (void) { isPaused = false; Invoke(pauseResume); pauseTime = 0;}
		bool IsPaused (void) const { return isPaused;}
		uint64_t GetPauseTime (void) const { return pauseTime;}
		uint64_t GetGameTime (void) const { return std::time(nullptr);}
};

void InstallPauseResumeHandler (Game& game);

#endif