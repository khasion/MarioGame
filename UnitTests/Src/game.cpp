#include "game.hpp"

ALLEGRO_BITMAP* getSprite(int id, ALLEGRO_BITMAP** sprite_map) {
	return sprite_map[id];
}

std::vector<std::vector<int>> readTextMap (ALLEGRO_BITMAP** sprite_map) {
	std::vector<std::vector<int>> v;
	std::vector<int> tempv;

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

void Game::MainLoop (ALLEGRO al) {
	bool done = false;
	bool redraw = true;
	ALLEGRO_EVENT event;

	std::vector<std::vector<int>> map = readTextMap(al.sprites.sprite_map);
	int x=0, y=0;
	int cnt = 0;
	al_start_timer(al.timer);
	while (true) {
		al_wait_for_event(al.queue, &event);
		switch (event.type) {
			case ALLEGRO_EVENT_TIMER:
				std::cout << x << "," << y << std::endl;
				if (al.key[ALLEGRO_KEY_HOME]) {
					y-=5;
				}
				if (al.key[ALLEGRO_KEY_END]) {
					y+=5;
				}
				redraw = true;
				break;
			case ALLEGRO_EVENT_MOUSE_AXES:
				x = event.mouse.x;
				redraw = true;
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				al.key[event.keyboard.keycode] = KEY_SEEN;
				break;
			case ALLEGRO_EVENT_KEY_UP:
				al.key[event.keyboard.keycode] &= KEY_RELEASED;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				done = true;
				break;
		}
		if (done) { break;}
		if (redraw && al_is_event_queue_empty(al.queue)) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			for (int i = 0; i < map.size(); i++) {
				for (int j = 0; j < map[i].size(); j++) {
					ALLEGRO_BITMAP* sprite = getSprite(map[i][j], al.sprites.sprite_map);
					al_draw_bitmap(sprite, x+j*TILE_W, y+i*TILE_H, 0);
				}
			}
			al_flip_display();
			redraw = false;
		}
	}
}

void Game::MainLoopIteration(void) {

}