#pragma once

#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "Scene.h"
#include "Player.h"
#include "Hazard.h"
#include "Map.h"
#include "Ram.h"
#include "SoundManager.h"
#include "Spawnable.h"
#include "TileWarning.h"
#include "ManHole.h"
#include <vector>

/*A subclass of Scene which handles the main game-loop.*/
class GameLoop : public Scene
{
public:
	// Objects variables
	std::vector<std::shared_ptr<Hazard>> hazards;
	std::unique_ptr<Player> player;
	Texture heart = Texture("Textures/Heart.png");
	Map map = Map("Textures/3x3_PlainMap.png", "Textures/3x3_PlainMapTopLayer.png", 3);

	GameLoop(GLFWwindow* window, Shader* shader, int viewportWidth = 1200, int viewportHeight = 800);
	~GameLoop();

	Scene* refresh();
	/* Handle user input for the Scene.*/
	void checkInputs();

	/*Create and return an instance of a random subclass of Hazard*/
	std::shared_ptr<Hazard> createRandomHazard();

private:
	
	Timer hazardSpawnTimer = Timer(9.0f);
	Timer inputTimer = Timer(0.5f);

	bool paused = false;
	int difficulty = 0;
};

#endif
