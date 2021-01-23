#ifndef APP_HPP
#define APP_HPP
#include "game.hpp"
#include "entities.hpp"

extern Entity* player;
extern Entity* goomba;
extern int dx, dy, fw;
extern float g, t;

namespace app {
	class App {
		protected:
		Game game;
		public:
		virtual void Initialize (void);
		virtual void Load (void);
		virtual void Run (void) {
			game.MainLoop();
		}
		virtual void RunIteration (void) {
			game.MainLoopIteration();
		}
		Game& GetGame (void) {return game; }
		virtual void Clear (void);
		void Main (void) {
			Initialize();
			Load();
			Run();
			Clear();
		}
	};
}

void InitPlayer ();
void InitEnemies ();
#endif