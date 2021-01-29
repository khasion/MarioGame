#ifndef ENT_HPP
#define ENT_HPP
#include "../Include/anim.hpp"

class Entity {
public:
	using OnDamage = std::function<void()>;
protected:
	Sprite*		sprite = nullptr;
	Animator*	animator = nullptr;
	OnDamage		onDamage;
	double		mi = 0, m = 0;
	double		g = 0.3;
	int			dx = 0, dy = 0, u = 0;
	int			startx = 0, starty = 0;
	int 			lives = 1;
	bool			isDying = false;
public:
	template <typename T>
	void SetOnDamage (const T& f)	{ onDamage = f;}

	virtual void 	Do (void) = 0;
	virtual void	Do (Sprite*) = 0;

	void 	Damage ();

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
	void		SetG(double _g)	{ g = _g;}
	void		ResetMass (void)	{ m = mi;}

	Point		GetStartPos (void){ return {startx, starty};}

	void 		SetDead (bool b)	{isDying = b;}
	bool		IsDead (void)		{ return isDying;}
	
	Entity (int _x, int _y, int _u, double _m, int _lives) : 
		startx(_x), starty(_y),
		u(_u), m(_m), mi(_m), lives(_lives) {}
};

class Mario : public Entity {	
private:
	Animator* flash 	= nullptr;
	Animator* death	= nullptr;
	bool 	isSuper		= false;
	bool 	isInv			= false;
	bool	isHit			= false;
	int	checkpoint 	= 320;
	int	coins 		= 0;
	int	points		= 0;
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	int  GetCoins (void)		{ return coins;}
	void SetCoins (int _c)	{ coins = _c;};
	void AddCoin (void)		{
		coins++;
		points += 200;
		al_play_sample(al.coin_sample,1,0,1,ALLEGRO_PLAYMODE_ONCE,NULL);
		if (coins >= 100) {
			lives++; coins -= 100;
		}
	}
	int GetPoints (void)		{ return points;}
	void SetPoints (int p)	{ points = p;}
	void AddPoints (int p)	{ points += p;}
	void SetHit (bool b)	{ isHit = b;}
	bool IsHit (void)		{ return isHit;}
	void SetSuper (bool b)	
			{ 	isSuper = b;
				if (b) {	points += 1000;}
			}
	bool IsSuper (void)		{ return isSuper;}
	void SetCheck (int x)	{ checkpoint = x;}
	int  GetCheck (void)		{ return checkpoint;}
	void SetInv (bool b)		
			{	isInv = b;
				if (b) {	points += 1000;}
			}
	bool IsInv (void)			{ return isInv;}
	Mario (int _x, int _y, int _u, double _m, int _lives) :
		Entity (_x, _y, _u, _m, _lives) {Init();};
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
	Sprite* GetColl (void) { return coll;}
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
	bool isHit = false;
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	bool IsHit (void) { return isHit;}
	void SetHit (bool b) { isHit = b;}
	Box (int _x, int _y, int _l) :
		Entity (_x, _y, 0, 0, _l) {Init();};
};

class Mushroom : public Entity {
private:
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	Mushroom (int _x, int _y, int _u, double _m, int _lives) :
		Entity (_x, _y, _u, _m, _lives) {Init();};
};

class OneUp : public Entity {
private:
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	OneUp (int _x, int _y, int _u, double _m, int _lives) :
		Entity (_x, _y, _u, _m, _lives) {Init();};
};

class Star : public Entity {
private:
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	Star (int _x, int _y, int _u, double _m, int _lives) :
		Entity (_x, _y, _u, _m, _lives) {Init();};
};

class Koopa : public Entity {
private:
	bool isRed = false;
	bool isStunned = false;
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	bool IsRed (void) { return isRed;}
	void SetRed (bool b) { isRed = b;}
	bool IsStunned (void) { return isStunned;}
	void SetStunned (bool b) { isStunned = b;}
	Koopa (int _x, int _y, int _u, double _m, int _lives, bool red) :
		Entity (_x, _y, _u, _m, _lives), isRed(red) {Init();};
};

class Brick : public Entity {
private:
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	Brick (int _x, int _y) :
		Entity (_x, _y, 0, 1, 1) {Init();}
};

class Teleporter : public Entity {
private:
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	Teleporter (int _x, int _y) :
		Entity (_x, _y, 0, 1, 1) {Init();}
};

class Squirrel : public Entity {
private:
public:
	void Do (void) override;
	void Do (Sprite*) override {};
	void Init (void);
	Squirrel (int _x, int _y, int _u, double _m, int _lives) :
		Entity (_x, _y, _u, _m, _lives) {Init();};
};

class EntityManager {
private:
	std::map<std::string, Entity*> entityMap;
	static EntityManager singleton;
	~EntityManager () {}
	EntityManager (void) {}
public:
	static EntityManager& Get(void) 	{ return singleton;}

	void 	Add (std::string str, Entity* e) 	{ entityMap.insert(std::make_pair(str, e));}
	void	Remove (std::string str, Entity* e)	{ entityMap.erase(str);}
	auto	GetAll (void)	{ return entityMap;}
	auto 	Get (std::string str) { return entityMap[str];}
};

#endif