#include "entities.hpp"

void Entity::Move (int* dx, int* dy) {
	if (onMove) onMove(dx, dy);
}

void Entity::Jump (int dx, int dy) {
	if (onJump) onJump(dx, dy);
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