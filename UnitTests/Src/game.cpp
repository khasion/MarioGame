#include "game.hpp"

void Game::MainLoop () {
	al_start_timer(al.timer);
	while (!IsFinished()) {
		al_wait_for_event(al.queue, &al.event);
		MainLoopIteration();
	}
}

void Game::MainLoopIteration(void) {
	Render();
	Input();
	ProgressAnimations();
	AI();
	Physics();
	CollisionChecking();
	CommitDestructions();
	UserCode();
}