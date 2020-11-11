#ifndef _VIGILANTE_INCLUDE
#define _VIGILANTE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <GL/glew.h>
#include <GL/glut.h>


class Vigilante
{
public:
	void init(ShaderProgram &shaderProgram);
	void render();
	bool update(int deltaTime, glm::vec2 posPlayer);
	bool pillado(glm::vec2 posPlayer);

	void move();

private:
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	glm::vec2 pos;
	bool wait;
	bool moving;
	int timeSinceStart;
	glm::vec2 posFin;
};

#endif