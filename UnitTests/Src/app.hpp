#ifndef APP_HPP
#define APP_HPP
#include "game.hpp"

extern Sprite* mario;

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
#endif