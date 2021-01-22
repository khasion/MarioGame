#include "entities.hpp"

void Entity::Move (int dx, int dy) {
	if (onJump) onJump(dx, dy);
}

void Entity::Jump (int dx, int dy) {
	if (onMove) onMove(dx, dy);
}

void Entity::Kill (int dx, int dy) {
	if (onDeath) onDeath(dx, dy);
}