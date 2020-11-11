#include "Vigilante.h"

enum BallAnims
{
	STAND_LEFT, STAND_RIGHT
};

void Vigilante::init(ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/vigilante.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 16), glm::vec2(0.5f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(0);

	sprite->setPosition(glm::vec2(48.f, 368.f));
	pos = glm::vec2(48.f, 368.f);
	wait = true;
	moving = false;
}

void Vigilante::move() {
	glm::vec2 rec = posFin - sprite->getPosition();
	double dist = sqrt(rec.x*rec.x + rec.y + rec.y);
	if (dist > 0) {
		pos.x += (posFin.x - sprite->getPosition().x) * 1 / dist;
		pos.y += (posFin.y - sprite->getPosition().y) * 1 / dist;
	}
	else {
		wait = true;
		moving = false;
	}
	sprite->setPosition(pos);
}

bool Vigilante::pillado(glm::vec2 posPlayer) {
	glm::vec2 posVigilante = sprite->getPosition();
	if (posVigilante.x >= posPlayer.x && posVigilante.x <= posPlayer.x + 48 && posVigilante.y >= posPlayer.y && posVigilante.y <= posPlayer.y + 48) return true;
	if (posVigilante.x - 32 >= posPlayer.x && posVigilante.x - 32 <= posPlayer.x + 48 && posVigilante.y >= posPlayer.y && posVigilante.y <= posPlayer.y + 48) return true;
	if (posVigilante.x >= posPlayer.x && posVigilante.x <= posPlayer.x + 48 && posVigilante.y - 16>= posPlayer.y && posVigilante.y - 16<= posPlayer.y + 48) return true;
	if (posVigilante.x - 32 >= posPlayer.x && posVigilante.x - 32 <= posPlayer.x + 48 && posVigilante.y - 16 >= posPlayer.y && posVigilante.y - 16 <= posPlayer.y + 48) return true;
	return false;
}

bool Vigilante::update(int deltaTime, glm::vec2 posPlayer) {
	if (wait) {
		timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
		wait = false;
	}
	else if (timeSinceStart + 3000 < glutGet(GLUT_ELAPSED_TIME) && !moving) {
		posFin.x = posPlayer.x + 32;
		posFin.y = posPlayer.y + 32;
		moving = true;
	}
	else if (moving) {
		move();
	}
	sprite->update(deltaTime);
	return pillado(posPlayer);
}

void Vigilante::render() {
	sprite->render();
}

