#include "Enemy.h"

#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/enemigo.png", x, y, 36, 40, game) {
	vida = 3;
	vx = -1;
}

void Enemy::update() {
	x = x + vx;

}

