#ifndef ENT_HPP
#define ENT_HPP
#include "anim.hpp"

class Entity {
public:
	using OnMove = std::function<void(int*, int*)>;
	using OnDeath = std::function<void(int, int)>;
private:
	Sprite*		sprite = nullptr;
	Animator*	animator = nullptr;
	OnMove 		onMove;
	OnDeath		onDeath;
	int			dx = 0, dy = 0;
	int 			lives = 1;
public:
	template <typename T>
	void SetOnMove (const T& f)	{ onMove = f;}
	template <typename T>
	void SetOnDeath (const T& f)	{ onDeath = f;}

	void 	Move (int* dx, int* dy);
	void 	Jump (int dx, int dy);
	void 	Kill (int dx, int dy);

	Sprite*	GetSprite (void)			{ return sprite;}
	void		SetSprite (Sprite* s)	{ sprite = s;}

	int 	GetLives (void) 	{ return lives;}
	void 	SetLives (int l)	{ lives = l;}

	void 			SetAnimator (Animator* anim);
	Animator*	GetAnimator (void) { return animator;}

	int	GetDx (void)	{ return dx;}
	int	GetDy (void)	{ return dy;}
	void 	SetDx (int x) {dx=x;}
	void 	SetDy (int y) {dy=y;}
	
	Entity (Sprite* _s, int _lives) : 
		sprite(_s), lives(_lives) {}
};

class EntityManager {
private:
	std::set<Entity*> entitySet;
	static EntityManager singleton;
public:
	static EntityManager Get(void) 	{ return singleton;}

	void 					Add (Entity* e) 	{ entitySet.insert(e);}
	std::set<Entity*>	GetAll (void)		{ return entitySet;}
};

#endif
