#ifndef ENT_HPP
#define ENT_HPP
#include "anim.hpp"

class Entity {
public:
	using OnJump = std::function<void(int, int)>;
	using OnMove = std::function<void(int, int)>;
	using OnDeath = std::function<void(int, int)>;
private:
	std::string type;
	OnJump 		onJump;
	OnMove 		onMove;
	OnDeath		onDeath;
	int 			lives = 1;
public:
	template <typename T>
	void SetOnJump (const T& f)	{ onJump = f;}
	template <typename T>
	void SetOnMove (const T& f)	{ onMove = f;}
	template <typename T>
	void SetOnDeath (const T& f)	{ onDeath = f;}

	void 	Move (int dx, int dy);
	void 	Jump (int dx, int dy);
	void 	Kill (int dx, int dy);

	int 	GetLives (void) 	{ return lives;}
	void 	SetLives (int l)	{ lives = l;}

	Entity (int _lives) : lives(_lives) {}
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
