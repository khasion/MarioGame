#ifndef GAME_HPP
#define GAME_HPP

#include "view.hpp"

#define DISP_W 640
#define DISP_H 480
#define MAX_FPS 200
#define FPS 1.0 / MAX_FPS

#define TILE_MAX 252
#define TILE_W 16
#define TILE_H 16
#define SHEET_WIDTH 12*TILE_W
#define SHEET_HEIGHT 21*TILE_H
#define TILE_MAP "../Media/map1_Kachelebene 1.csv"

#define KEY_SEEN 1
#define KEY_RELEASED 2

ALLEGRO_BITMAP* getSprite (int);
std::vector<std::vector<int>> readTextMap (void);

class Game {
	public:
		using Action 	= std::function<void(void)>;
		using Pred		= std::function<bool(void)>;
	private:
		Action render, anim, input, ai, physics, destruct, collisions, user;
		Pred 	done;
		void 	Invoke (const Action& f) { if (f) f();}
	public:
		// Setters
		template <typename Tfunc>
		void setRender (const Tfunc& f) {render = f;}
		template <typename Tfunc>
		void setAnim (const Tfunc& f) {anim = f;}
		template <typename Tfunc>
		void setInput (const Tfunc& f) {input = f;}
		template <typename Tfunc>
		void setAI (const Tfunc& f) {ai = f;}
		template <typename Tfunc>
		void setPhysics (const Tfunc& f) { physics = f;}
		template <typename Tfunc>
		void setDestruct (const Tfunc& f) { destruct = f;}
		template <typename Tfunc>
		void setCollisions (const Tfunc& f) { collisions = f;}
		template <typename Tfunc>
		void setUser (const Tfunc& f) { user = f;}
		template <typename Tfunc>
		void setDone (const Tfunc& f) {done = f;}
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
};
#endif