#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &sizeNotBlock);
	int getVidas();
	void setVidas(int n);
	int updateVidas();
	double PositionOfPlayerWhereColision(glm::vec2 ballPos, glm::vec2 ballSize);
	glm::vec2 getPos();
	
private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	glm::vec2 size;
	int vidas;

};


#endif // _PLAYER_INCLUDE


