#include "../Headers/GameLoop.h"
#include "../Headers/Menu.h"


bool* Spawnable::availableTiles = new bool[] {
	false, false, false,
		false, false, false,
		false, false, false,
};
Map* Spawnable::map = nullptr;

GameLoop::GameLoop(GLFWwindow* window, Shader* shader, int viewportWidth, int viewportHeight)
	: Scene(window, shader, viewportWidth, viewportHeight)
{
	std::cout << "\nGameLoop Scene created";

	player = std::make_unique<Player>("Textures/Player.png", *window, 0.0f, 0.0f, &map);

	SoundManager::Instance();
	SoundManager::Instance()->soundEngine->play2D("SFX/SwipeTheme.wav", true);

	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, &glm::scale(screenScaling, glm::vec3(4.0f, 4.0f, 1.0f))[0][0]);

	Spawnable::map = &map;
}

GameLoop::~GameLoop() 
{
	hazards.clear();
	SoundManager::Instance()->soundEngine->stopAllSounds();
	Spawnable::map = nullptr;
}

Scene* GameLoop::refresh()
{
	// Update clocks
	calculateUpdatePeriod();
	inputTimer.updateClock(updatePeriod);
	hazardSpawnTimer.updateClock(updatePeriod);

	
	checkInputs(); //Check for any input

	if (player->lives < 1)
	{
		return new Menu(&window, &shader, (int)viewportDimensions.x, (int)viewportDimensions.y); //Return a new Menu if the player dies
	}

	if (!paused)
	{
		difficulty += (int)(updatePeriod / 10);

		if (hazardSpawnTimer.getTimeIsUp())
		{
			if (difficulty < 3)
			{
				srand((unsigned int)glfwGetTime());
				hazardSpawnTimer.setDuration(rand() % (5 - difficulty) + 1.0f); //Randomly change hazardSpawnTimer
			}

			for (int i = 0; i <= hazards.size(); i++)
			{
				// if (no Hazards are available)
				if (i == hazards.size())
				{
					hazards.push_back(createRandomHazard()); //Create a new hazard
				}
				
				if (hazards[i]->state == Hazard::STATE::READY)
				{
					hazards[i]->activate();
					break; //Stop searching for more Hazards
				}
			}
			hazardSpawnTimer.reset();
		}		

		player->update(updatePeriod);

		for (int i = 0; i < hazards.size(); i++)
		{
			hazards[i]->update(updatePeriod);

			if (hazards[i]->hasCompletedIteration)
			{
				hazards.erase(hazards.begin() + i--);
			}
			else if (player->collider.isCollidingWith(hazards[i]->collider))
			{
				player->handleCollisionWith(hazards[i]->collider);
			}
		}

		// Updates every TileWarning Object
		for (TileWarning* warning : map.tileWarnings)
		{
			warning->update(updatePeriod);
		}
	}

	drawTexture(map.texture, glm::vec3(0.0f), 0.0f, glm::vec3(1.0f)); //Render the bottom layer of the map
	drawTexture(player->texture, glm::vec3(player->xPos, player->yPos, 0.0f), player->rotationRadians, glm::vec3(1.0f)); //Render the player
	
	// Render every Object
	for (int i = 0; i < hazards.size(); i++) 
	{ 
		drawTexture(hazards[i]->texture, glm::vec3(hazards[i]->xPos, hazards[i]->yPos, 0.0f), hazards[i]->rotationRadians, glm::vec3(1.0f)); 
	}

	// Render a TileWarning Object if it is active
	for (TileWarning* warning : map.tileWarnings)
	{
		if (warning->isActive)
		{
			drawTexture(warning->texture, glm::vec3(warning->xPos, warning->yPos, 0.0f), 0, glm::vec3(1.0f));
		}
	}

	drawTexture(map.topTexture, glm::vec3(0.0f), 0.0f, glm::vec3(1.0f)); //Render the top layer of the map

	// Render the number of lives represented by hearts
	for (int i = 0; i < player->lives; i++)
	{
		drawTexture(heart, glm::vec3(((heart.Width() + 3) * (i + 1)) - ((viewportDimensions.x / 2.0f) + heart.Width() / 2.0f), ((viewportDimensions.y / 2.0f) - (heart.Height() / 2.0f)), 0.0f), 0, glm::vec3(1.0f));
	}

	return nullptr;
}

void GameLoop::checkInputs()
{
	if (inputTimer.numberOfTicksPassed() == 0) return;

	if (glfwGetKey(&window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		paused = !paused;
		std::cout << ((paused) ? "\nPausing" : "\nContinuing Game");
		inputTimer.reset();
	}
}

std::shared_ptr<Hazard> GameLoop::createRandomHazard()
{
	srand((unsigned int)glfwGetTime());
	switch (rand() % 3)
	{
	case 0:
		return std::make_shared<Ram>("Textures/PlainSpike.png");
	case 1:
		return std::make_shared<ManHole>("Textures/PlainManHoleSpritesheet.png");
	default:
		std::cout << "\nMaking beam";
		return std::make_shared<Ram>("Textures/PlainSpike.png");
	}
}