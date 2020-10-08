#pragma once

#include "Actor.h"
#include "Projectile.h" 
#include "Audio.h"

class Player : public Actor
{
public:
	Player(string image,float x, float y, int sizeX, int sizeY, int shootCadence,float v, Game* game);
	Projectile* shoot();
	Audio* audioShoot;
	int playerLifes = 3;
	float v = 0;

	void loseLife();
	bool isDead();
	void update();
	void moveX(float axis);
	void moveY(float axis);
	int shootCadence=30;
	int shootTime = 0;

};

