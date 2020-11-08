#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Block.h"

Block *Block::createBlock(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program, int tipe, const glm::vec2 &pos, int n_animations, const glm::vec2 &spritePos)
{
	Block *quad = new Block(quadSize, sizeInSpritesheet, spritesheet, program, tipe, pos, n_animations, spritePos);

	return quad;
}


Block::Block(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program, int tipe, const glm::vec2 &pos, int n_animations, const glm::vec2 &spritePos)
{
	sprite = new Sprite(quadSize, sizeInSpritesheet, spritesheet, program);
	this->tipe = tipe;
	size = quadSize;
	position = pos;
	sprite->setPosition(pos);
	sprite->setNumberAnimations(n_animations);
	if (tipe == 1) {
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, spritePos);
		sprite->changeAnimation(0);
	}
	else if (tipe == 17) {
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, spritePos);
		sprite->changeAnimation(0);
	}
	else if (tipe == 3) {
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, spritePos);

		sprite->setAnimationSpeed(1, 8);
		sprite->addKeyframe(1, glm::vec2(spritePos.x - 0.125f, spritePos.y));

		sprite->changeAnimation(0);
	}
	else if (tipe == 19) {
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, spritePos);

		sprite->setAnimationSpeed(1, 8);
		sprite->addKeyframe(1, glm::vec2(spritePos.x - 0.125f, spritePos.y));
		
		sprite->changeAnimation(0);
	}
	else {
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, spritePos);
		sprite->changeAnimation(0);
	}
	
}

void Block::update(int deltaTime)
{
	sprite->update(deltaTime);
}

void Block::resistencia() {
	if (tipe == 3) {
		sprite->changeAnimation(1);
		tipe = 1;
	}
	else if (tipe == 19) {
		sprite->changeAnimation(1);
		tipe = 17;
	}
}

void Block::render() const
{
	sprite->render();
}

void Block::free()
{
	sprite->free();
}

bool Block::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &sizeNotBlock)
{
	glm::vec2 posBlock = sprite->getPosition();
	if (pos.x + sizeNotBlock.x - 1 >= posBlock.x && pos.x + sizeNotBlock.x -1 <= posBlock.x + size.x -1 &&
		!(pos.y >= posBlock.y + size.y -1 || pos.y + sizeNotBlock.y - 1 <= posBlock.y)) {
		return true;
	}
	return false;
}

bool Block::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &sizeNotBlock)
{
	glm::vec2 posBlock = sprite->getPosition();
	if (pos.x >= posBlock.x && pos.x  <= posBlock.x + size.x - 1 &&
		!(pos.y >= posBlock.y + size.y - 1 || pos.y + sizeNotBlock.y - 1 <= posBlock.y))
	{
		return true;
	}
	return false;
}

bool Block::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &sizeNotBlock)
{
	glm::vec2 posBlock = sprite->getPosition();
	if (pos.y + sizeNotBlock.y - 1 >= posBlock.y && pos.y + sizeNotBlock.y - 1 <= posBlock.y + size.y - 1 &&
		!(pos.x >= posBlock.x + size.x -1 || pos.x + sizeNotBlock.x - 1 <= posBlock.x))
	{
		return true;
	}
	return false;
}

bool Block::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &sizeNotBlock)
{
	glm::vec2 posBlock = sprite->getPosition();
	if (pos.y  >= posBlock.y && pos.y <= posBlock.y + size.y -1 &&
		!(pos.x >= posBlock.x + size.x - 1 || pos.x + sizeNotBlock.x - 1 <= posBlock.x))
	{
		return true;
	}
	return false;
}

int Block::getTipe() {
	return tipe;
}

glm::vec2 Block::getPosition() {
	return position;
}