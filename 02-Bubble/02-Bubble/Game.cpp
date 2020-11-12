#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	engine = createIrrKlangDevice();
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if (key == '1')
		scene.canviaPantalla(1);
	if (key == '2')
		scene.canviaPantalla(2);
	if (key == '3')
		scene.canviaPantalla(3);
	if (key == 'q' || Game::instance().getKey('Q'))
		scene.setNivel(1);
	if (key == 'w' || Game::instance().getKey('W'))
		scene.setNivel(2);
	if (key == 'e' || Game::instance().getKey('E'))
		scene.setNivel(3);
	if (key == 'g' || Game::instance().getKey('G'))
		scene.god_mode();
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

ISoundEngine* Game::getEngine()
{
	return engine;
}





