#include "Player.h"

Player::Player(string image,float x, float y, int sizeX, int sizeY, int shootCadence,float v,Game* game) 
	:Actor(image,x,y,sizeX,sizeY,game){
	audioShoot = new Audio("res/efecto_disparo.wav", false);
	this->shootCadence = shootCadence;
	this->v = v;
}
void Player::update() {
	if (shootTime > 0) {
		shootTime--;
	}
	x = x + vx;
	y = y + vy;
}

void Player::moveX(float axis) {
	vx = axis * v;
}

void Player::moveY(float axis) {
	vy = axis * v;
}

Projectile* Player::shoot() {
	if (shootTime == 0) {
		audioShoot->play();
		shootTime = shootCadence;
		return new Projectile(x, y, game);
	}
	else {
		return NULL;
	}
}

void Player::loseLife() {
	if (!isDead()) {
		playerLifes--;
	}
}

bool Player::isDead() {
	return playerLifes == 0;
}

//nullptr