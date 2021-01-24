#include "entities.hpp"

EntityManager EntityManager::singleton;

void Entity::Move () {
	if (onMove) onMove();
}

void Entity::Kill (int dx, int dy) {
	if (onDeath) onDeath(dx, dy);
}

void Entity::SetAnimator (Animator* anim) {
	if (animator) {
		animator->Stop();
		AnimatorManager::GetSingleton().Cancel(animator);
		free(animator);
	}
	animator = anim;
}