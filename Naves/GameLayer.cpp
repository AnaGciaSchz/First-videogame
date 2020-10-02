#include "GameLayer.h"

GameLayer::GameLayer(Game* game): Layer(game){ //llamamos al super
	init();
}

void GameLayer::init() {
	delete player; //antes de crear nuevo personaje, se borra anterior

	tiles.clear(); //se limpian al inicial el nivel

	points = 0;
	textPoints = new Text("0", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);

	background = new Background("res/fondo_2.png", WIDTH * 0.5, HEIGHT * 0.5,-1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game); //Va a estar en el 85% de la x y el 5% de y


	projectiles.clear(); // Vaciar por si reiniciamos el juego

	enemies.clear(); // Vaciar por si reiniciamos el juego
	loadMap("res/0.txt"); //ahora lo cargamos todo a partir del mapa 
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
	background->update();
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

				enemy->impacted();
				points++;
				textPoints->content = to_string(points);

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


		for (auto const& projectile : projectiles) {
			if (projectile->isInRender() == false) {

				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {//
					deleteProjectiles.push_back(projectile);
				}
			}
		}

	}

	for (auto const& enemy : enemies) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();

			if (!eInList) {
				deleteEnemies.push_back(enemy);
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


void GameLayer::loadMap(string name) {
	char character; //si no hay puntero se pone en el stack
	string line; //se ha creado un objeto dentro del stack, es similar a ponerlo con el new (pero new se hace en el heap)
	//new devuelve una posición en memoria en el heap
	ifstream streamFile(name.c_str());//el constructor se llama en la variable directamente
	if (!streamFile.is_open()) {
		cout << "Fallo al abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}
			cout << character << endl;
		}
	}
	streamFile.close();
}

void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
	case 'E': {
		Enemy* enemy = new Enemy(x, y, game);
		// modificación para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		break;
	}
	case '1': {
		player = new Player(x, y, game);
		// modificación para empezar a contar desde el suelo.
		player->y = player->y - player->height / 2;
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/bloque_tierra.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		break;
	}
	}
}


void GameLayer::draw() {
	background->draw();

	for (auto const& tile : tiles) {
		tile->draw();
	}
	//escribir int const a = 0 es lo mismo que const int a = 0
	for (auto const& projectile : projectiles) {
		projectile->draw();
	}
	player->draw();

	for (auto const& enemy : enemies) {
		enemy->draw();
	}
	textPoints->draw();
	backgroundPoints->draw(); //Lo pintamos el último para que los enemigos no lo tapen tampoco

	SDL_RenderPresent(game->renderer); // Renderiza
}
