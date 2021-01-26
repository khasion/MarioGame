#ifndef ENT_HPP
#define ENT_HPP
#include "anim.hpp"

class Entity {
public:
	using OnDeath = std::function<void()>;
protected:
	Sprite*		sprite = nullptr;
	Animator*	animator = nullptr;
	OnDeath		onDeath;
	double		mi = 0, m = 0;
	double		g = 0.7;
	int			dx = 0, dy = 0, u = 0;
	int			startx = 0, starty = 0;
	int 			lives = 1;
	int 			coins = 0;
	bool			isDead = false;
public:
	template <typename T>
	void SetOnDeath (const T& f)	{ onDeath = f;}

	virtual void 	Do (void) = 0;
	virtual void	Do (Sprite*) = 0;

	void 	Damage ();

	Sprite*	GetSprite (void)			{ return sprite;}
	void		SetSprite (Sprite* s)	{ sprite = s;}
	int 	GetCoins (void)		{ return coins;}
	void 	SetCoins (int c)	{ coins = c;}

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
	void 		SetCoins (int _c)	{c = _c};
	void		ResetMass (void)	{ m = mi;}
	
	Entity (int _x, int _y, int _u,int _c double _m, int _coins,int _lives) : 
		startx(_x), starty(_y),
		u(_u), m(_m), mi(_m), lives(_lives) {}
};

class Mario : public Entity {	
private:
	Animator* flash 	= nullptr;
	Animator* death	= nullptr;
	bool 	isSuper		= false;
	bool 	isInv			= false;
	int	checkpoint 	= 320;
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	Mario (int _x, int _y, int _u, double _m,int _coins int _lives) :
		Entity (_x, _y, _u, _m,_coins , _lives) {Init();};
};

class Goomba : public Entity {
private:
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	Goomba (int _x, int _y, int _u, double _m, int _lives) :
		Entity (_x, _y, _u, _m, _lives) {Init();};
};

class Piranha : public Entity {
private:
	Sprite* coll;
public:
	void Do (void) override {};
	void Do (Sprite*) override;
	void Init (void);
	Piranha (int _x, int _y, int _lives) :
		Entity (_x, _y, 0, 0, _lives) { Init();};
};

class Coin : public Entity {
private:
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	Coin (int _x, int _y) :
		Entity (_x, _y, 0, 0, 1) {Init();};
};
class Box : public Entity {
private:
	int number_of_coins=0;
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	Coin (int _x, int _y,int number_of_coins) :
		Entity (_x, _y, 0, 0, 1) {Init();};
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