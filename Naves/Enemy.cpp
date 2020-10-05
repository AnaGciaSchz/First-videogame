#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/enemigo.png", x, y, 36, 40, game) {

	vx = -1;

	aMoving = new Animation("res/enemigo_movimiento.png", width, height,
		108, 40, 6, 3, game);
	animation = aMoving;

}

void Enemy::update() {
	// Actualizar la animación
	animation->update();

	x = x + vx;
}

void Enemy::draw() {
	animation->draw(x, y);
}

void Enemy::loseLife() {
	if (!isDead()) {
		enemyLifes--;
	}
}

bool Enemy::isDead() {
	return enemyLifes == 0;
}