#pragma once
#include "Actor.h"
#include "Animation.h" 

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void update();
	void draw() override; // Va a sobrescribir
	void loseLife();
	bool isDead();

	int enemyLives = 3;
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada

};

