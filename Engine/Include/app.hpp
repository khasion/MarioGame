#ifndef APP_HPP
#define APP_HPP
#include "../Include/game.hpp"
#include "../Include/entities.hpp"

extern Mario* player;
extern Entity* piranha;
extern Entity* enemy_1;
extern ALLEGRO_SAMPLE* coin_sample;
extern ALLEGRO_SAMPLE* jump_sample;
extern double g;

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
void InitGoomba ();
void InitPiranha ();
void InitPlayer ();
void InitCoinAndBoxes ();
void InitCollisions ();
void InitKoopas ();
void InitPowerUps ();
#endif
