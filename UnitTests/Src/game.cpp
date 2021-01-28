#include "game.hpp"

std::vector<std::string> messages;

void Game::MainLoop () {
	
	al_start_timer(al.timer);
	Pause(std::time(nullptr));
	messages.push_back("GIANNIS KASSIONIS, APOSTOLIS NIKOLAOU");
	messages.push_back("University of Crete");
	messages.push_back("Department of Computer Science");
	messages.push_back("Development of Intelligent Interfaces and Games");
	messages.push_back("Term Project, Fall Semester 2020");
	messages.push_back("Press ENTER to play!");
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