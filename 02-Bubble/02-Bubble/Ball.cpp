#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Ball.h"
#include "Game.h"


#define PI 3.1415



void Ball::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/pilota.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(18, 20), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	size = glm::vec2(18, 20);
	texProgram = shaderProgram;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y -3)));
	angle = PI/3;
	stopped = true;
	speed = 3;
}

void Ball::update(int deltaTime, bool *alarma, int *money, int *points, bool jugar)
{
	if (jugar && Game::instance().getKey(' ') || Game::instance().getSpecialKey(GLUT_KEY_LEFT) || Game::instance().getSpecialKey(GLUT_KEY_RIGHT) ||
		Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		stopped = false;
	}
	if (!stopped) {
		glm::dvec2 oldPosBall = posBall;
		sprite->update(deltaTime);
		posBall.x += cos(angle) * speed;
		posBall.y -= sin(angle) * speed;
		double new_angle = angle;

	
	for (int i = 0;i < (*blocks).size() && new_angle==angle;++i){
		Block block = (*blocks)[i];
		if (angle <= PI / 2) { //primer quadrant
			if (block.collisionMoveUp(sprite->getPosition(), glm::vec2(18, 20))) {
				new_angle = 2 * PI - angle;
				if (block.getTipe() == 15 || block.getTipe() == 3 || block.getTipe() == 19 || block.getTipe() == 1 || block.getTipe() == 17) Game::instance().getEngine()->play2D("audio/solid.wav");
				if (block.getTipe() == 7) {
					if(!*alarma) alarmSound = Game::instance().getEngine()->play2D("audio/alarm2.wav", true, false, true);
					*alarma = true;
				}
				else if (block.getTipe() != 15 && !(*blocks)[i].resistencia()) {
					if (block.getTipe() == 1 || block.getTipe() == 3 || block.getTipe() == 17 || block.getTipe() == 19) *points += 100;
					else if (block.getTipe() == 5) {
						*money += 100;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 9) {
						*money += 200;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 11) {
						*money += *points;
						*points = 0;
						Game::instance().getEngine()->play2D("audio/smw_power-up_appears.wav");
					}
					(*blocks).erase((*blocks).begin() + i);
				}
				if (block.getTipe() == 13) {
					agafaClau();
					Game::instance().getEngine()->play2D("audio/smw_keyhole_exit.wav");
				}
			}
			else if (block.collisionMoveRight(sprite->getPosition(), glm::vec2(18, 20))) {
				new_angle = PI - angle;
				if (block.getTipe() == 15 || block.getTipe() == 3 || block.getTipe() == 19 || block.getTipe() == 1 || block.getTipe() == 17) Game::instance().getEngine()->play2D("audio/solid.wav");
				if (block.getTipe() == 7) {
					if (!*alarma) alarmSound = Game::instance().getEngine()->play2D("audio/alarm2.wav", true, false, true);
					*alarma = true;
				}
				else if (block.getTipe() != 15 && !(*blocks)[i].resistencia()) {
					if (block.getTipe() == 1 || block.getTipe() == 3 || block.getTipe() == 17 || block.getTipe() == 19) *points += 100;
					else if (block.getTipe() == 5) {
						*money += 100;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 9) {
						*money += 200;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 11) {
						*money += *points;
						*points = 0;
						Game::instance().getEngine()->play2D("audio/smw_power-up_appears.wav");
					}
					(*blocks).erase((*blocks).begin() + i);
				}
				if (block.getTipe() == 13) {
					agafaClau();
					Game::instance().getEngine()->play2D("audio/smw_keyhole_exit.wav");
				}
			}
		}
		else if (angle >= PI / 2 && angle <= PI) //segon quadrant
		{
			if (block.collisionMoveUp(sprite->getPosition(), glm::vec2(18, 20))) {
				new_angle = 2 * PI - angle;
				if (block.getTipe() == 15 || block.getTipe() == 3 || block.getTipe() == 19 || block.getTipe() == 1 || block.getTipe() == 17) Game::instance().getEngine()->play2D("audio/solid.wav");
				if (block.getTipe() == 7) {
					if (!*alarma) alarmSound = Game::instance().getEngine()->play2D("audio/alarm2.wav", true, false, true);
					*alarma = true;
				}
				else if (block.getTipe() != 15 && !(*blocks)[i].resistencia()) {
					if (block.getTipe() == 1 || block.getTipe() == 3 || block.getTipe() == 17 || block.getTipe() == 19) *points += 100;
					else if (block.getTipe() == 5) {
						*money += 100;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 9) {
						*money += 200;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 11) {
						*money += *points;
						*points = 0;
						Game::instance().getEngine()->play2D("audio/smw_power-up_appears.wav");
					}
					(*blocks).erase((*blocks).begin() + i);
				}
				if (block.getTipe() == 13) {
					agafaClau();
					Game::instance().getEngine()->play2D("audio/smw_keyhole_exit.wav");
				}
			}
			else if (block.collisionMoveLeft(sprite->getPosition(), glm::vec2(18, 20))) {
				new_angle = PI - angle;
				if (block.getTipe() == 15 || block.getTipe() == 3 || block.getTipe() == 19 || block.getTipe() == 1 || block.getTipe() == 17) Game::instance().getEngine()->play2D("audio/solid.wav");
				if (block.getTipe() == 7) {
					if (!*alarma) alarmSound = Game::instance().getEngine()->play2D("audio/alarm2.wav", true, false, true);
					*alarma = true;
				}
				else if (block.getTipe() != 15 && !(*blocks)[i].resistencia()) {
					if (block.getTipe() == 1 || block.getTipe() == 3 || block.getTipe() == 17 || block.getTipe() == 19) *points += 100;
					else if (block.getTipe() == 5) {
						*money += 100;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 9) {
						*money += 200;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 11) {
						*money += *points;
						*points = 0;
						Game::instance().getEngine()->play2D("audio/smw_power-up_appears.wav");
					}
					(*blocks).erase((*blocks).begin() + i);
				}
				if (block.getTipe() == 13) {
					agafaClau();
					Game::instance().getEngine()->play2D("audio/smw_keyhole_exit.wav");
				}
			}
		}
		else if (angle >= PI && angle <= 3 * PI / 2) //tercer quadrant
		{
			if (player->collisionMoveDown(sprite->getPosition(), glm::vec2(18, 20))) {
				if (!godMode && player->PositionOfPlayerWhereColision(sprite->getPosition(), size) > 0.9) {
					new_angle = 330 * PI / 180;
					speed = 5;
				} else {
					new_angle = 90 * PI / 180 - player->PositionOfPlayerWhereColision(sprite->getPosition(), size) * 45 * PI / 180;
					if (new_angle > (90 + 30)*PI / 180 || new_angle < (90 - 30)*PI / 180) speed = 5;
					else speed = 3;
				}
				Game::instance().getEngine()->play2D("audio/bleep.wav");
			}
			else if (block.collisionMoveDown(sprite->getPosition(), glm::vec2(18, 20))) {
				if (block.getTipe() == 15 || block.getTipe() == 3 || block.getTipe() == 19 || block.getTipe() == 1 || block.getTipe() == 17) Game::instance().getEngine()->play2D("audio/solid.wav");
				new_angle = PI - (angle - PI);
				if (block.getTipe() == 7) {
					if (!*alarma) alarmSound = Game::instance().getEngine()->play2D("audio/alarm2.wav", true, false, true);
					*alarma = true;
				}
				else if (block.getTipe() != 15 && !(*blocks)[i].resistencia()) {
					if (block.getTipe() == 1 || block.getTipe() == 3 || block.getTipe() == 17 || block.getTipe() == 19) *points += 100;
					else if (block.getTipe() == 5) {
						*money += 100;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 9) {
						*money += 200;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 11) {
						*money += *points;
						*points = 0;
						Game::instance().getEngine()->play2D("audio/smw_power-up_appears.wav");
					}
					(*blocks).erase((*blocks).begin() + i);
				}
				if (block.getTipe() == 13) {
					agafaClau();
					Game::instance().getEngine()->play2D("audio/smw_keyhole_exit.wav");
				}
			}
			else if (block.collisionMoveLeft(sprite->getPosition(), glm::vec2(18, 20))) {
				new_angle = 2 * PI - (angle - PI);
				if (block.getTipe() == 15 || block.getTipe() == 3 || block.getTipe() == 19 || block.getTipe() == 1 || block.getTipe() == 17) Game::instance().getEngine()->play2D("audio/solid.wav");
				if (block.getTipe() == 7) {
					if (!*alarma) alarmSound = Game::instance().getEngine()->play2D("audio/alarm2.wav", true, false, true);
					*alarma = true;
				}
				else if (block.getTipe() != 15 && !(*blocks)[i].resistencia()) {
					if (block.getTipe() == 1 || block.getTipe() == 3 || block.getTipe() == 17 || block.getTipe() == 19) *points += 100;
					else if (block.getTipe() == 5) {
						*money += 100;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 9) {
						*money += 200;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 11) {
						*money += *points;
						*points = 0;
						Game::instance().getEngine()->play2D("audio/smw_power-up_appears.wav");
					}
					(*blocks).erase((*blocks).begin() + i);
				}
				if (block.getTipe() == 13) {
					agafaClau();
					Game::instance().getEngine()->play2D("audio/smw_keyhole_exit.wav");
				}
			}
		}
		else if (angle >= 3 * PI / 2) //quart quadrant
		{
			if (player->collisionMoveDown(sprite->getPosition(), glm::vec2(18, 20))) {
				if (!godMode && player->PositionOfPlayerWhereColision(sprite->getPosition(), size) < 0.1) {
					new_angle = 210 * PI / 180;
					speed = 5;
				}
				else {
					new_angle = 90 * PI / 180 - player->PositionOfPlayerWhereColision(sprite->getPosition(), size) * 45 * PI / 180;
					if (new_angle > (90 + 30)*PI / 180 || new_angle < (90 - 30)*PI / 180) speed = 5;
					else speed = 3;
				}
				Game::instance().getEngine()->play2D("audio/bleep.wav");
			}
			if (block.collisionMoveDown(sprite->getPosition(), glm::vec2(18, 20))) {
				new_angle = 2 * PI - angle;
				if (block.getTipe() == 15 || block.getTipe() == 3 || block.getTipe() == 19 || block.getTipe() == 1 || block.getTipe() == 17) Game::instance().getEngine()->play2D("audio/solid.wav");
				if (block.getTipe() == 7) {
					if (!*alarma) alarmSound = Game::instance().getEngine()->play2D("audio/alarm2.wav", true, false, true);
					*alarma = true;
				}
				else if (block.getTipe() != 15 && !(*blocks)[i].resistencia()) {
					if (block.getTipe() == 1 || block.getTipe() == 3 || block.getTipe() == 17 || block.getTipe() == 19) *points += 100;
					else if (block.getTipe() == 5) {
						*money += 100;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 9) {
						*money += 200;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 11) {
						*money += *points;
						*points = 0;
						Game::instance().getEngine()->play2D("audio/smw_power-up_appears.wav");
					}
					(*blocks).erase((*blocks).begin() + i);
				}
				if (block.getTipe() == 13) {
					agafaClau();
					Game::instance().getEngine()->play2D("audio/smw_keyhole_exit.wav");
				}
			}
			else if (block.collisionMoveRight(sprite->getPosition(), glm::vec2(18, 20))) {
				new_angle = 3 * PI / 2 - (angle - 3 * PI / 2);
				if (block.getTipe() == 15 || block.getTipe() == 3 || block.getTipe() == 19 || block.getTipe() == 1 || block.getTipe() == 17) Game::instance().getEngine()->play2D("audio/solid.wav");
				if (block.getTipe() == 7) {
					if (!*alarma) alarmSound = Game::instance().getEngine()->play2D("audio/alarm2.wav", true, false, true);
					*alarma = true;
				}
				else if (block.getTipe() != 15 && !(*blocks)[i].resistencia()) {
					if (block.getTipe() == 1 || block.getTipe() == 3 || block.getTipe() == 17 || block.getTipe() == 19) *points += 100;
					else if (block.getTipe() == 5) {
						*money += 100;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 9) {
						*money += 200;
						Game::instance().getEngine()->play2D("audio/coin.wav");
					}
					else if (block.getTipe() == 11) {
						*money += *points;
						*points = 0;
						Game::instance().getEngine()->play2D("audio/smw_power-up_appears.wav");
					}
					(*blocks).erase((*blocks).begin() + i);
				}
				if (block.getTipe() == 13) {
					agafaClau();
					Game::instance().getEngine()->play2D("audio/smw_keyhole_exit.wav");
				}
			}
		}
	}
	if (new_angle != angle) {
		angle = new_angle;
		posBall.x = oldPosBall.x + cos(angle) * speed;
		posBall.y = oldPosBall.y - sin(angle) * speed;
	}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	}
}

bool Ball::ganastebro() {
	for (int i = 0; i < (*blocks).size(); ++i) 
		if ((*blocks)[i].getTipe() == 5 || (*blocks)[i].getTipe() == 9) return false;
	
	return true;
}

void Ball::agafaClau() {
	for (int i = 15; i > 7; --i)
		(*blocks).erase((*blocks).begin() + i);
}

glm::dvec2 Ball::getPos() {
	return posBall;
}

void Ball::render()
{
	sprite->render();
}

void Ball::setAngle(double a) {
	angle = a;
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
void Ball::setBlocks(vector<Block> *newBlocks)
{
	blocks = newBlocks;
}
void Ball::setPlayer(Player *newPlayer)
{
	player = newPlayer;
}

void Ball::setStopped(bool b) {
	stopped = b;
}

void Ball::setGodMode(bool b) {
	godMode = b;
}

void Ball::stopAlarma() {
	alarmSound->stop();
}
