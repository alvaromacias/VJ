#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Block.h"


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

private:
	void initShaders();
	void createBlockMap(const string &levelFile);
	void createBlockSprites(vector<Sprite> &);
	void renderBlocks(vector<Sprite> &);
	void setPantalla(int n);
	

private:
	TileMap *map;
	string spritesheetFile;
	int *blockMap;
	Player *player;
	Ball *ball;
	vector<Sprite> blocks_1;
	vector<Sprite> blocks_2;
	vector<Sprite> blocks_3;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Texture spritesheet;
	int pantalla;

};


#endif // _SCENE_INCLUDE

