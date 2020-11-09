#ifndef _BLOCK_INCLUDE
#define _BLOCK_INCLUDE

#include <vector>
#include <glm/glm.hpp>
#include "Sprite.h"

class Block
{

public:
	static Block *createBlock(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program, int block, const glm::vec2 &pos, int n_animations, const glm::vec2 &spritePos);

	Block(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program, int block, const glm::vec2 &pos, int n_animations, const glm::vec2 &spritePos);

	void update(int deltaTime);
	void render() const;
	void free();

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &sizeNotBlock);
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &sizeNotBlock);
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &sizeNotBlock);
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &sizeNotBlock);

	int getTipe();
	glm::vec2 getPosition();
	bool resistencia();

private:
	Sprite *sprite;
	int tipe;
	glm::vec2 size;
	glm::vec2 position;

};


#endif // _SPRITE_INCLUDE
