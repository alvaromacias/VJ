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
	sprite->setPosition(pos);
	sprite->setNumberAnimations(n_animations);
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, spritePos);
	sprite->changeAnimation(0);
}

void Block::update(int deltaTime)
{
	sprite->update(deltaTime);
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