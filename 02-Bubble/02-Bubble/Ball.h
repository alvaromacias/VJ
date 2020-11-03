#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the ball.


class Ball
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

private:
	bool bJumping;
	glm::ivec2 tileMapDispl;
	glm::dvec2 posBall;
	int jumpAngle, startY;
	double angle;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _BALL_INCLUDE