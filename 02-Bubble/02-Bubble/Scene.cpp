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
	createBlockMap("levels/level1_1.txt");
	createBlockSprites();
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2((INIT_PLAYER_X_TILES +1) * map->getTileSize(), (INIT_PLAYER_Y_TILES - 2) * map->getTileSize()));
	player->setTileMap(map);

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

void Scene::createBlockSprites() {
	glm::vec2 spritePos;
	blocks = vector<Sprite>();
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
					Sprite *sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125f, 1.f), &spritesheet, &texProgram);
					sprite->setPosition(glm::vec2(16 * i + 32, 16 * j + 16));
					sprite->setNumberAnimations(1);
					sprite->setAnimationSpeed(0, 8);
					sprite->addKeyframe(0, spritePos);
					sprite->changeAnimation(0);
					blocks.push_back(*sprite);
				}
				//crea sprites de la pared (blocs grocs) (blocs de 16x16)
				if (blockMap[j * 24 + i] == 15) {
					Sprite *sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125f, 1.f), &spritesheet, &texProgram);
					sprite->setPosition(glm::vec2(16 * i + 32, 16 * j + 16));
					sprite->setNumberAnimations(1);
					sprite->setAnimationSpeed(0, 8);
					sprite->addKeyframe(0, spritePos);
					sprite->changeAnimation(0);
					blocks.push_back(*sprite);
				}
				//crea els ladrillos destruibles (blocs de 32x16)
				else {
					Sprite *sprite = Sprite::createSprite(glm::vec2(32, 16), glm::vec2(0.125f, 0.5f), &spritesheet, &texProgram);
					sprite->setPosition(glm::vec2(16 * i + 32, 16 * j + 16));
					sprite->setNumberAnimations(1);
					sprite->addKeyframe(0, spritePos);
					sprite->changeAnimation(0);
					blocks.push_back(*sprite);
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
	renderBlocks();
	player->render();
	ball->render();
}

void Scene::renderBlocks() {
	for (int i = 0; i < blocks.size(); ++i) {
		blocks[i].render();
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



