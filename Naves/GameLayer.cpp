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
	if (event.type == SDL_QUIT) {
		game->loopActive = false;
	}
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE: //Salir del juego
			game->loopActive = false;
			break;
		case SDLK_1: //Poner en pantalla completa
			game->scale();
			break;
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
	// Generar enemigos
	newEnemyTime--;
	if (newEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Enemy(rX, rY, game));
		newEnemyTime = std::min(200-killedEnemys,50); //Cada vez que se mata un enemigo, estos aparecen antes
	}

	player->update();
	for (auto const& enemy : enemies) { //auto es como var o dynamic, infiere tipo
		enemy->update();
	}
	// Colisiones, Player - Enemy
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			init();
			return; // Cortar el for
		}
	}

	// Colisiones , Enemy - Projectile

	//Listas temporales para anotar qué queremos eliminar (y no borrar de las listas mientras las recorremos)
	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;

	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) { //Si le da un proyectil o se sale de la pantalla
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end(); //comprobar si el proyectil ya estaba en la lista

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}

				bool eInList = std::find(deleteEnemies.begin(),
					deleteEnemies.end(),
					enemy) != deleteEnemies.end(); //comprobar si el enemigo ya estaba en la lista

				if (!eInList) {
					deleteEnemies.push_back(enemy);
				}

			}
			if (enemy->x + enemy->width / 2 <= 0) { //si el enemigo se sale de la pantalla
				bool eInList = std::find(deleteEnemies.begin(),
					deleteEnemies.end(),
					enemy) != deleteEnemies.end(); //comprobar si el enemigo ya estaba en la lista

				if (!eInList) {
					deleteEnemies.push_back(enemy);
				}

			}

			if (projectile->x - projectile->width / 2 >= WIDTH) { //si el proyectil se sale de la pantalla
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end(); //comprobar si el proyectil ya estaba en la lista

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
			}
		}
	}

	//Eliminamos los enemigos y proyectiles que han colisionado
	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
	}
	deleteProjectiles.clear();



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
