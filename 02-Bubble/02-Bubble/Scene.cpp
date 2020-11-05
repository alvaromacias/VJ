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
	initShaders();
	map = TileMap::createTileMap("levels/background.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2((INIT_PLAYER_X_TILES + 1) * map->getTileSize(), (INIT_PLAYER_Y_TILES - 1.25) * map->getTileSize()));
	ball->setTileMap(map);

	createBlockMap("levels/level1_1.txt");
	createBlockSprites(blocks_1);
	ball->setBlocks(blocks_1);
	createBlockMap("levels/level1_2.txt");
	createBlockSprites(blocks_2);
	createBlockMap("levels/level1_3.txt");
	createBlockSprites(blocks_3);
	setPantalla(1);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	ball->setPlayer(player);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
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
	for (int j = 0; j < mapSize.y; j++)
	{
		getline(fin, line);
		sstream.str(line);
		for (int i = 0; i < mapSize.x; i++)
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
	for (int i = 0; i < 24; ++i) {
		for (int j = 0; j < 24; ++j) {
			if (blockMap[j * 24 + i] % 2 != 0 && blockMap[j * 24 + i] != 29) {
				if (blockMap[j * 24 + i] < 16) {
					spritePos.x = (blockMap[j * 24 + i]-1.f) / 16.f;
					spritePos.y = 0.f;
				}
				else {
					spritePos.x = (blockMap[j * 24 + i] - 17.f) / 16.f;
					spritePos.y = 0.5f;
				}
				//crea sprites de diners, alarm, telefon/calculadora el q sigui i clau (blocs de 32x32)
				if (blockMap[j * 24 + i] == 5 || blockMap[j * 24 + i] == 7 || blockMap[j * 24 + i] == 9 || blockMap[j * 24 + i] == 11 || blockMap[j * 24 + i] == 13) {
					Block *block = Block::createBlock(glm::ivec2(32, 32), glm::vec2(0.125f, 1.f), &spritesheet, &texProgram, blockMap[j * 24 + i], glm::vec2(16 * i + 32, 16 * j + 16), 1, spritePos);
					
					blocks.push_back(*block);
				}
				//crea sprites de la pared (blocs grocs) (blocs de 16x16)
				if (blockMap[j * 24 + i] == 15) {
					Block *block = Block::createBlock(glm::ivec2(16, 16), glm::vec2(0.125f, 1.f), &spritesheet, &texProgram, blockMap[j * 24 + i], glm::vec2(16 * i + 32, 16 * j + 16), 1, spritePos);
					blocks.push_back(*block);
				}
				//crea els ladrillos destruibles (blocs de 32x16)
				else {
					Block *block = Block::createBlock(glm::vec2(32, 16), glm::vec2(0.125f, 0.5f), &spritesheet, &texProgram, blockMap[j * 24 + i], glm::vec2(16 * i + 32, 16 * j + 16), 1, spritePos);

					blocks.push_back(*block);
				}
			}
		}
	}
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	ball->update(deltaTime);
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
	map->render();
	if (pantalla == 1) renderBlocks(blocks_1);
	else if (pantalla == 2) renderBlocks(blocks_2);
	else if (pantalla ==3) renderBlocks(blocks_3);
	player->render();
	ball->render();
}

void Scene::renderBlocks(vector<Block> &blocks) {
	for (int i = 0; i < blocks.size(); ++i) {
		blocks[i].render();
	}
}

//metode public
void Scene::canviaPantalla(int n) {
	setPantalla(n);
}

//metode privat
void Scene::setPantalla(int n) {
	pantalla = n;
	if (pantalla == 1) {
		ball->setBlocks(blocks_1);
	}
	else if (pantalla == 2) {
		ball->setBlocks(blocks_2);
	}
	else if (pantalla == 3) {
		ball->setBlocks(blocks_3);
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



