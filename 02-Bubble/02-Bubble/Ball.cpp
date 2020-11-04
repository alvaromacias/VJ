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



void Ball::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/pilota.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(18, 20), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y -3)));
	angle = PI/3;

}

void Ball::update(int deltaTime)
{
	glm::dvec2 oldPosBall = posBall;
	sprite->update(deltaTime);
	posBall.x += cos(angle) * SPEED;
	posBall.y -= sin(angle) * SPEED;
	double new_angle = angle;

	
	for (int i = 0;i < blocks.size() && new_angle==angle;++i){
		Block block = blocks[i];
		if (angle <= PI / 2) { //primer quadrant
			if (block.collisionMoveUp(sprite->getPosition(), glm::vec2(18, 20))) new_angle = 2 * PI - angle;
			else if (block.collisionMoveRight(sprite->getPosition(), glm::vec2(18, 20))) new_angle = PI - angle;
		}
		else if (angle >= PI / 2 && angle <= PI) //segon quadrant
		{
			if (block.collisionMoveUp(sprite->getPosition(), glm::vec2(18, 20))) new_angle = 2*PI - angle;
			else if (block.collisionMoveLeft(sprite->getPosition(), glm::vec2(18, 20))) new_angle = PI - angle;
		}
		else if (angle >= PI && angle <= 3 * PI / 2) //tercer quadrant
		{
			if (player->collisionMoveDown(sprite->getPosition(), glm::vec2(18, 20))) new_angle = PI - (angle - PI);
			else if (block.collisionMoveDown(sprite->getPosition(), glm::vec2(18, 20))) new_angle = PI - (angle - PI);
			else if (block.collisionMoveLeft(sprite->getPosition(), glm::vec2(18, 20))) new_angle = 2 * PI - (angle - PI);
		}
		else if (angle >= 3 * PI / 2) //quart quadrant
		{
			if (player->collisionMoveDown(sprite->getPosition(), glm::vec2(18, 20))) new_angle = 2 * PI - angle;
			if (block.collisionMoveDown(sprite->getPosition(), glm::vec2(18, 20))) new_angle = 2 * PI - angle;
			else if (block.collisionMoveRight(sprite->getPosition(), glm::vec2(18, 20))) new_angle = 3 * PI / 2 - (angle - 3 * PI / 2);
		}
	}
	if (new_angle != angle) {
		angle = new_angle;
		posBall.x = oldPosBall.x + cos(angle) * SPEED;
		posBall.y = oldPosBall.y - sin(angle) * SPEED;
	}

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
void Ball::setBlocks(vector<Block> &newBlocks)
{
	blocks = newBlocks;
}
void Ball::setPlayer(Player *newPlayer)
{
	player = newPlayer;
}
