#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"


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

private:
	void initShaders();
	void createBlockMap(const string &levelFile);
	void createBlockSprites();
	void renderBlocks();

private:
	TileMap *map;
	int *blockMap;
	Player *player;
	vector<Sprite> blocks;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Texture spritesheet;

};


#endif // _SCENE_INCLUDE

