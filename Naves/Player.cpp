#include "Player.h"

Player::Player(float x, float y, Game* game) :Actor("res/jugador.png",x,y,50,57,game){

}
void Player::update() {
	if (shootTime > 0) {
		shootTime--;
	}

	int xResult = x + vx;

	if (xResult - width / 2 > 0 && xResult + width / 2 < WIDTH) {
		x = xResult;
	}

	int yResult = y + vy;

	if (yResult - height / 2 > 0 && yResult + height / 2 < HEIGHT) {
		y = yResult;
	}
}

void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}

Projectile* Player::shoot() {
	if (shootTime == 0) {
		shootTime = shootCadence;
		return new Projectile(x, y, game);
	}
	else {
		return NULL;
	}
}

//nullptr