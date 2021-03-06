#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 11
#define INIT_PLAYER_Y_TILES 21

#define VIDAS 5


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
}


void Scene::init()
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	initShaders();
	map = TileMap::createTileMap("levels/fondo.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	negroQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 1) * map->getTileSize(), (INIT_PLAYER_Y_TILES - 1.25) * map->getTileSize()));
	ball->setTileMap(map);

	blocks_1.clear();
	blocks_2.clear();
	blocks_3.clear();
	createBlockMap("levels/level1_1.txt");
	createBlockSprites(blocks_1);
	ball->setBlocks(&blocks_1);
	createBlockMap("levels/level1_2.txt");
	createBlockSprites(blocks_2);
	createBlockMap("levels/level1_3.txt");
	createBlockSprites(blocks_3);
	first = true;
	nivel = 1;
	setPantalla(1);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	ball->setPlayer(player);
	vidas = VIDAS;
	god = false;
	ball->setGodMode(false);
	money = 0;
	points = 0;
	restart = false;
	menu = true;
	instrucciones = false;
	jugar = false;
	creditos = false;

	vigilante = new Vigilante();
	vigilante->init(texProgram);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	//if (!text.init("fonts/Vintage.ttf"))
	//if(!text.init("fonts/OpenSans-Bold.ttf"))
	if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Scene::createBlockMap(const string &levelFile) {
	ifstream fin;
	string line, spritesheetFile;
	stringstream sstream;
	glm::ivec2 mapSize;
	glm::vec2 spriteSize;

	fin.open(levelFile.c_str());

	getline(fin, line);

	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> spriteSize.x >> spriteSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> spritesheetFile;
	spritesheet.loadFromFile(spritesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	getline(fin, line);
	sstream.str(line);
	sstream >> spriteSize.x >> spriteSize.y;
	spriteSize = glm::vec2(1.f / spriteSize.x, 1.f / spriteSize.y);


	blockMap = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.x; j++)
	{
		getline(fin, line);
		sstream.str(line);
		for (int i = 0; i < mapSize.y; i++)
		{
			sstream >> blockMap[j*mapSize.x + i];
		}
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

}

void Scene::createBlockSprites(vector<Block> &blocks) {
	glm::vec2 spritePos;
	spritesheet.loadFromFile("images/sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	for (int j = 0; j < 24; ++j) {
		for (int i = 0; i < 24; ++i) {
			if (blockMap[j * 24 + i] % 2 != 0 && blockMap[j * 24 + i] != 29 && blockMap[j*24 + i] != 27 && blockMap[j * 24 + i] != 25 && blockMap[j * 24 + i] != 23 && blockMap[j * 24 + i] != 21) {
				if (blockMap[j * 24 + i] < 16) {
					spritePos.x = (blockMap[j * 24 + i]-1.f) / 16.f;
					spritePos.y = 0.f;
				}
				else {
					spritePos.x = (blockMap[j * 24 + i] - 17.f) / 16.f;
					spritePos.y = 0.5f;
				}
				//crea sprites de diners, alarma, telefon/calculadora el q sigui i clau (blocs de 32x32)
				if (blockMap[j * 24 + i] == 5 || blockMap[j * 24 + i] == 7 || blockMap[j * 24 + i] == 9 || blockMap[j * 24 + i] == 11 || blockMap[j * 24 + i] == 13) {
					Block *block = Block::createBlock(glm::ivec2(32, 32), glm::vec2(0.125f, 1.f), &spritesheet, &texProgram, blockMap[j * 24 + i], glm::vec2(16 * i + 32, 16 * j + 16), 1, spritePos);
					
					blocks.push_back(*block);
				}
				//crea sprites de la pared (blocs grocs) (blocs de 16x16)
				else if (blockMap[j * 24 + i] == 15) {
					Block *block = Block::createBlock(glm::ivec2(16, 16), glm::vec2(0.125f, 1.f), &spritesheet, &texProgram, blockMap[j * 24 + i], glm::vec2(16 * i + 32, 16 * j + 16), 1, spritePos);
					blocks.push_back(*block);
				}
				//crea els ladrillos destruibles (blocs de 32x16)
				else {
					Block *block = Block::createBlock(glm::vec2(32, 16), glm::vec2(0.125f, 0.5f), &spritesheet, &texProgram, blockMap[j * 24 + i], glm::vec2(16 * i + 32, 16 * j + 16), 3, spritePos);

					blocks.push_back(*block);
				}
			}
		}
	}
}

void Scene::die() {
	if (vidas == 0) {
		jugar = false;
		restart = true;
		Game::instance().getEngine()->play2D("audio/smw_game_over.wav");
	}
	else {
		if (!god) --vidas;
		player->setPosition(glm::vec2(176.f, 336.f));
		ball->setStopped(true);
		ball->setPosition(glm::vec2(192.f, 320.f));
		ball->setAngle(3.1415 / 3);
		if (alarma) {
			vigilante->init(texProgram);
			alarma = false;
			ball->stopAlarma();
		}
		setPantalla(1);
	}
}

bool Scene::pantallaVigilante() {
	if (pantalla == 3 && nivel == 2) return true;
	if (pantalla == 2 && nivel == 3) return true;
	return false;
}

void Scene::update(int deltaTime)
{
	if (!restart) {
		currentTime += deltaTime;
		if (ball->getPos().y < 3) {
			avanzaPantalla(pantalla + 1);
			Game::instance().getEngine()->play2D("audio/smw_1-up.wav");
		}
		if (ball->getPos().y > 370 && pantalla > 1) {
			avanzaPantalla(pantalla - 1);
			Game::instance().getEngine()->play2D("audio/smw_koopa_kid_falls_into_lava.wav");
		}
		else if (ball->getPos().y > 370) die();
		if (pantalla == 3 && ball->ganastebro()) {
			if (nivel == 3) {
				creditos = true;
				jugar = false;
			}
			else setNivel(nivel + 1);
		}
		player->update(deltaTime);
		ball->update(deltaTime, &alarma, &money, &points, jugar);
		if (alarma && pantallaVigilante()) {
			if (vigilante->update(deltaTime, player->getPos())) die();
		}
	}
}

void Scene::setNivel(int n) {
	nivel = n;
	if (n == 1) init();
	else if (n == 2) createNivel2();
	else if (n == 3) createNivel3();
	if (alarma) ball->stopAlarma();
	alarma = false;
	vigilante->init(texProgram);
}

void Scene::createNivel2() {
	createBlockMap("levels/level2_1.txt");
	blocks_1.clear();
	createBlockSprites(blocks_1);
	ball->setBlocks(&blocks_1);
	createBlockMap("levels/level2_2.txt");
	blocks_2.clear();
	createBlockSprites(blocks_2);
	createBlockMap("levels/level2_3.txt");
	blocks_3.clear();
	createBlockSprites(blocks_3);
	first = true;
	nivel = 2;
	vidas = VIDAS;
	setPantalla(1);
	player->setPosition(glm::vec2(176.f, 336.f));
	ball->setPosition(glm::vec2(192.f, 320.f));
	ball->setStopped(true);
}

void Scene::createNivel3() {
	createBlockMap("levels/level3_1.txt");
	blocks_1.clear();
	createBlockSprites(blocks_1);
	ball->setBlocks(&blocks_1);
	createBlockMap("levels/level3_2.txt");
	blocks_2.clear();
	createBlockSprites(blocks_2);
	createBlockMap("levels/level3_3.txt");
	blocks_3.clear();
	createBlockSprites(blocks_3);
	first = true;
	nivel = 3;
	vidas = VIDAS;
	setPantalla(1);
	player->setPosition(glm::vec2(176.f, 336.f));
	ball->setPosition(glm::vec2(192.f, 320.f));
	ball->setStopped(true);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	if (menu) {
		texs.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
		texQuad->render(texs);
		text.render("PRESS 'P' TO PLAY", glm::vec2(8 * map->getTileSize(), 22 * map->getTileSize()), 32, glm::vec4(0, 0, 1, 1), projection);
		text.render("PRESS 'I' FOR INSTRUCTIONS", glm::vec2(8 * map->getTileSize(), 24 * map->getTileSize()), 32, glm::vec4(0, 0, 1, 1), projection);
		text.render("PRESS 'C' FOR CREDITS", glm::vec2(8 * map->getTileSize(), 26 * map->getTileSize()), 32, glm::vec4(0, 0, 1, 1), projection);
		if (Game::instance().getKey('i') || Game::instance().getKey('I')) instrucciones = true;
		else if (Game::instance().getKey('p') || Game::instance().getKey('P')) jugar = true;
		else if (Game::instance().getKey('c') || Game::instance().getKey('C')) creditos = true;
	}
	if (jugar) {
		menu = false;
		map->render();
		if (pantalla == 1) renderBlocks(blocks_1);
		else if (pantalla == 2) renderBlocks(blocks_2);
		else if (pantalla == 3) renderBlocks(blocks_3);
		player->render();
		ball->render();
		if (alarma && pantallaVigilante()) vigilante->render();

		text.render("MONEY:", glm::vec2(27 * map->getTileSize(), 2 * map->getTileSize() + 8), 32, glm::vec4(1, 1, 1, 1), projection);
		text.render(to_string(money), glm::vec2(27 * map->getTileSize(), 4 * map->getTileSize() + 8), 32, glm::vec4(1, 1, 1, 1), projection);

		text.render("POINTS:", glm::vec2(27 * map->getTileSize(), 7 * map->getTileSize() + 8), 32, glm::vec4(1, 1, 1, 1), projection);
		text.render(to_string(points), glm::vec2(27 * map->getTileSize(), 9 * map->getTileSize() + 8), 32, glm::vec4(1, 1, 1, 1), projection);

		text.render("LIVES:", glm::vec2(27 * map->getTileSize(), 12 * map->getTileSize() + 8), 32, glm::vec4(1, 1, 1, 1), projection);
		text.render(to_string(vidas), glm::vec2(27 * map->getTileSize(), 14 * map->getTileSize() + 8), 32, glm::vec4(1, 1, 1, 1), projection);

		text.render("BANK:", glm::vec2(27 * map->getTileSize(), 17 * map->getTileSize() + 8), 32, glm::vec4(1, 1, 1, 1), projection);
		text.render(to_string(nivel), glm::vec2(27 * map->getTileSize(), 19 * map->getTileSize() + 8), 32, glm::vec4(1, 1, 1, 1), projection);

		text.render("ROOM:", glm::vec2(27 * map->getTileSize(), 22 * map->getTileSize() + 8), 32, glm::vec4(1, 1, 1, 1), projection);
		text.render(to_string(pantalla), glm::vec2(27 * map->getTileSize(), 24 * map->getTileSize() + 8), 32, glm::vec4(1, 1, 1, 1), projection);
	}

	if (restart) {
		texs.loadFromFile("images/negro.png", TEXTURE_PIXEL_FORMAT_RGBA);
		negroQuad->render(texs);
		text.render("GAME OVER!", glm::vec2(8 * map->getTileSize(), 10 * map->getTileSize()), 64, glm::vec4(1, 0, 0, 1), projection);
		text.render("PRESS 'R' TO RESTART", glm::vec2(4 * map->getTileSize(), 14 * map->getTileSize()), 32, glm::vec4(1, 0, 0, 1), projection);
		text.render("PRESS 'M' TO GO TO THE MENU", glm::vec2(4 * map->getTileSize(), 16 * map->getTileSize()), 32, glm::vec4(1, 0, 0, 1), projection);
		if (Game::instance().getKey('r') || Game::instance().getKey('R')) {
			init();
			menu = false;
			jugar = true;
			restart = false;
		}
		else if (Game::instance().getKey('m') || Game::instance().getKey('M')) init();
	}
	if (instrucciones) {
		menu = false;
		texs.loadFromFile("images/negro.png", TEXTURE_PIXEL_FORMAT_RGBA);
		negroQuad->render(texs);
		text.render("INSTRUCTIONS", glm::vec2(4 * map->getTileSize(), 7 * map->getTileSize()), 64, glm::vec4(1, 0, 0, 1), projection);
		text.render("Press 1, 2, 3 to navigate through", glm::vec2(4 * map->getTileSize(), 13 * map->getTileSize()), 28, glm::vec4(1, 0, 0, 1), projection);
		text.render("the rooms of a bank", glm::vec2(8 * map->getTileSize(), 15 * map->getTileSize()), 28, glm::vec4(1, 0, 0, 1));
		text.render("Press 'Q', 'W', 'E' to navigate through banks", glm::vec2(4 * map->getTileSize(), 17 * map->getTileSize()), 28, glm::vec4(1, 0, 0, 1), projection);
		text.render("Press 'G' to activate god mode", glm::vec2(4 * map->getTileSize(), 19 * map->getTileSize()), 28, glm::vec4(1, 0, 0, 1), projection);
		text.render("Use the arrows to move the player", glm::vec2(4 * map->getTileSize(), 21 * map->getTileSize()), 28, glm::vec4(1, 0, 0, 1), projection);
		text.render("Press ESP to get the ball rolling", glm::vec2(4 * map->getTileSize(), 23 * map->getTileSize()), 28, glm::vec4(1, 0, 0, 1), projection);
		text.render("Press 'M' to go back to menu", glm::vec2(10 * map->getTileSize(), 29 * map->getTileSize()), 20, glm::vec4(1, 0, 0, 1), projection);
		if (Game::instance().getKey('m') || Game::instance().getKey('M')) init();
	}
	if (creditos) {
		menu = false;
		texs.loadFromFile("images/negro.png", TEXTURE_PIXEL_FORMAT_RGBA);
		negroQuad->render(texs);
		text.render("CREDITS", glm::vec2(8 * map->getTileSize(), 7 * map->getTileSize()), 64, glm::vec4(1, 0, 0, 1), projection);
		text.render("Created by:", glm::vec2(4 * map->getTileSize(), 13 * map->getTileSize()), 28, glm::vec4(1, 0, 0, 1), projection);
		text.render("Alvaro Macias", glm::vec2(10 * map->getTileSize(), 15 * map->getTileSize()), 28, glm::vec4(1, 0, 0, 1), projection);
		text.render("Marta Gil", glm::vec2(10 * map->getTileSize(), 17 * map->getTileSize()), 28, glm::vec4(1, 0, 0, 1), projection);
		text.render("Based on Break In by ALTECO", glm::vec2(4 * map->getTileSize(), 21 * map->getTileSize()), 28, glm::vec4(1, 0, 0, 1), projection);
		text.render("Press 'M' to go back to menu", glm::vec2(10 * map->getTileSize(), 29 * map->getTileSize()), 20, glm::vec4(1, 0, 0, 1), projection);
		if (Game::instance().getKey('m') || Game::instance().getKey('M')) init();
	}
}

void Scene::renderBlocks(vector<Block> &blocks) {
	for (int i = 0; i < blocks.size(); ++i) {
		blocks[i].render();
	}
}

int Scene::getPantalla() {
	return pantalla;
}

//metode public
void Scene::canviaPantalla(int n) {
	setPantalla(n);
}

void Scene::god_mode() {
	god = true;
	ball->setGodMode(true);
}


void Scene::avanzaPantalla(int n) {
	if (n == 1) {
		if(!first) ball->setPosition(glm::vec2(192.f, 16.f));
		ball->setBlocks(&blocks_1);
	}
	else if (n == 2) {
		first = false;
		if (pantalla == 1) ball->setPosition(glm::vec2(192.f, 368.f));
		else if (pantalla == 3) ball->setPosition(glm::vec2(192.f, 16.f));
		ball->setBlocks(&blocks_2);
	}
	else if (n == 3) {
		first = false;
		ball->setPosition(glm::vec2(192.f, 368.f));
		ball->setBlocks(&blocks_3);
	}
	pantalla = n;
}

void Scene::setPantalla(int n) {
	pantalla = n;
	if (pantalla == 1) {
		ball->setPosition(glm::vec2(192.f, 320.f));
		ball->setBlocks(&blocks_1);
	}
	else if (pantalla == 2) {
		ball->setPosition(glm::vec2(192.f, 320.f));
		ball->setBlocks(&blocks_2);
	}
	else if (pantalla == 3) {
		ball->setPosition(glm::vec2(192.f, 320.f));
		ball->setBlocks(&blocks_3);
	}
}


void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



