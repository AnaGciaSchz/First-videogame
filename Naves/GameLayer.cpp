#include "GameLayer.h"

GameLayer::GameLayer(Game* game): Layer(game){ //llamamos al super
	init();
}

void GameLayer::init() {
	delete player; //antes de crear nuevo personaje, se borra anterior
	player = new Player(50,50,game); // new = se crea en el heap y no se borra
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, game);

	projectiles.clear(); // Vaciar por si reiniciamos el juego

	enemies.clear(); // Vaciar por si reiniciamos el juego
	enemies.push_back(new Enemy(300, 50, game));
	enemies.push_back(new Enemy(300, 200, game)); //crea instancia de enemy, la copia y mete la copia, al final tenemos 2
	//emplace_back (Enemy...) ´con esto nos ahorramos las copias

}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) { //como no es un puntero, pasamos referencia
		keysToControls(event);
	}
	//procesar controles
	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			projectiles.push_back(newProjectile);
		}
	}
	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}
	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}


}

//Necesario diferenciar cuando se pulsa y
//levanta para cambiar valores y que no parezca que
//está siempre pulsada
void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}


	}

}

void GameLayer::update() {
	player->update();
	for (auto const& enemy : enemies) { //auto es como var o dynamic, infiere tipo
		enemy->update();
	}
	// Colisiones
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			init();
			return; // Cortar el for
		}
	}

	for (auto const& projectile : projectiles) {
		projectile->update();
	}

	std::cout << "update GameLayer" << std::endl;
}

void GameLayer::draw() {
	background->draw();
	for (auto const& projectile : projectiles) {
		projectile->draw();
	}
	player->draw();

	for (auto const& enemy : enemies) {
		enemy->draw();
	}


	SDL_RenderPresent(game->renderer); // Renderiza
}
