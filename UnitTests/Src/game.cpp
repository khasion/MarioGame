#include "game.hpp"

ALLEGRO_BITMAP* getSprite(int id) {
	return al.map.tilemap[id];
}

std::vector<std::vector<int>> readTextMap () {
	std::vector<std::vector<int>> v;
	std::vector<int> tempv;

	ALLEGRO_BITMAP** sprite_map = al.map.tilemap;

	std::ifstream file(TILE_MAP);
	int i = 0;
	std::string input;
	while (file >> input) {
		std::string temp;
		v.push_back(tempv);
		for (auto it = input.cbegin(); it!=input.cend(); ++it) {
			if (*it == ',') {
				v[i].push_back(std::stoi(temp));
				temp.clear();
			}
			else {
				temp.push_back(*it);
			}
		}
		i++;
	}
	return v;
}

void Game::MainLoop () {
	int loopCounter = 0;
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