#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Block.h"
#include "Player.h"


// Ball is basically a Sprite that represents the ball.


class Ball
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setBlocks(vector<Block> *newBlocks);
	void setPlayer(Player *newPlayer);
	void agafaClau();
	glm::dvec2 getPos();
	bool ganastebro();
	void setStopped(bool b);
	void setAngle(double a);

private:
	glm::ivec2 tileMapDispl;
	glm::dvec2 posBall;
	double angle;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	vector<Block> *blocks;
	Player *player;
	bool stopped;
	ShaderProgram texProgram;
};


#endif // _BALL_INCLUDE