#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Ball.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define SPEED 1
#define PI 3.1415


enum BallAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Ball::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/pilota.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(18, 20), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	angle = PI/3;

}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);
	posBall.x += sin(angle) * SPEED;
	posBall.y -= cos(angle) * SPEED;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::render()
{
	sprite->render();
}

void Ball::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Ball::setPosition(const glm::vec2 &pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}
