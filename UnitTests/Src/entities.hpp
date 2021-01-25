#ifndef ENT_HPP
#define ENT_HPP
#include "anim.hpp"

class Entity {
public:
	using OnMove = std::function<void(void)>;
	using OnDeath = std::function<void()>;
private:
	Sprite*		sprite = nullptr;
	Animator*	animator = nullptr;
	OnMove 		onMove;
	OnDeath		onDeath;
	double		mi = 0, m = 0;
	double		g = 0.7;
	int			dx = 0, dy = 0, u = 0;
	int 			lives = 1;
public:
	template <typename T>
	void SetOnMove (const T& f)	{ onMove = f;}
	template <typename T>
	void SetOnDeath (const T& f)	{ onDeath = f;}

	void 	Move ();
	void 	Die ();

	Sprite*	GetSprite (void)			{ return sprite;}
	void		SetSprite (Sprite* s)	{ sprite = s;}

	int 	GetLives (void) 	{ return lives;}
	void 	SetLives (int l)	{ lives = l;}

	void 			SetAnimator (Animator* anim);
	Animator*	GetAnimator (void) { return animator;}

	int*		GetDx (void)		{ return &dx;}
	int*		GetDy (void)		{ return &dy;}
	double	GetMass (void)		{ return m;}
	int		GetSpeed(void)		{ return u;}
	double	GetG(void)			{ return g;}
	void	 	SetDx (int _dx) 	{ dx = _dx;}
	void	 	SetDy (int _dy) 	{ dy = _dy;}
	void		SetMass(double _m){ m = _m;}
	void 		SetSpeed (int _u)	{ u = _u;}
	void		SetG(int _g)		{ g = _g;}
	void		ResetMass (void)	{ m = mi;}
	
	Entity (Sprite* _s, int _u, double _m, int _lives) : 
		sprite(_s), u(_u), m(_m), mi(_m), lives(_lives) {}
};

class EntityManager {
private:
	std::set<Entity*> entitySet;
	static EntityManager singleton;
	~EntityManager () {}
	EntityManager (void) {}
public:
	static EntityManager& Get(void) 	{ return singleton;}

	void 					Add (Entity* e) 	{ entitySet.insert(e);}
	void					Remove (Entity* e){ entitySet.erase(e);}
	std::set<Entity*>	GetAll (void)		{ return entitySet;}
};

#endif