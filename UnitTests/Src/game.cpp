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
	if (!IsPaused()) {
		ProgressAnimations();
		AI();
		Physics();
		CollisionChecking();
		CommitDestructions();
		UserCode();
	}
}

void InstallPauseResumeHandler (Game& game) {
	game.SetOnPauseResume(
		[&game](void) {
			if (!game.IsPaused()) {
				AnimatorManager::GetSingleton().TimeShift(game.GetGameTime() - game.GetPauseTime());
			}
		}
	);
}