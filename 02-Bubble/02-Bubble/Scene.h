#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Block.h"
#include "Vigilante.h"
#include "Text.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void canviaPantalla(int n);
	int getPantalla();
	void setNivel(int n);
	void avanzaPantalla(int n);
	void god_mode();

private:
	void initShaders();
	void createBlockMap(const string &levelFile);
	void createBlockSprites(vector<Block> &);
	void renderBlocks(vector<Block> &);
	void setPantalla(int n);
	void createNivel2();
	void createNivel3();
	void die();
	bool pantallaVigilante();
	

private:
	TileMap *map;
	string spritesheetFile;
	int *blockMap;
	Player *player;
	Ball *ball;
	Vigilante *vigilante;
	vector<Block> blocks_1;
	vector<Block> blocks_2;
	vector<Block> blocks_3;
	bool first;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Texture spritesheet;
	int pantalla;
	int nivel;
	int vidas;
	int money;
	int points;
	bool alarma;
	bool god;
	Text text;
};


#endif // _SCENE_INCLUDE

