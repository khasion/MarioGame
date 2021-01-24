#include "entities.hpp"

EntityManager EntityManager::singleton;

void Entity::Move () {
	if (onMove) onMove();
}

void Entity::Die () {
	if (onDeath) onDeath();
}

void Entity::SetAnimator (Animator* anim) {
	if (animator) {
		animator->Stop();
		AnimatorManager::GetSingleton().Cancel(animator);
		free(animator);
	}
	animator = anim;
}