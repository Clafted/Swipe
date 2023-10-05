#include "../Headers/PlayerController.h"
#include "../Headers/Player.h"

PlayerController::PlayerController(Player* contextObject, GLFWwindow& window) 
	: Component(contextObject), window(window), moveCooldownTimer(0.2f) {}

void PlayerController::update(float updatePeriod)
{
	float xMovement = 0.0f, yMovement = 0.0f;
	this->updatePeriod = updatePeriod;

	moveCooldownTimer.updateClock(updatePeriod);

	if (moveCooldownTimer.numberOfTicksPassed() > 0)
	{
		if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS)
		{ yMovement = 50.0f; }
		else if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS)
		{ yMovement = -50.0f; }
		else if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS)
		{ xMovement = -50.0f; }
		else if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS)
		{ xMovement = 50.0f; }

		
		if (xMovement != 0.0f || yMovement != 0.0f)
		{
			contextObject.moveTarget(xMovement, yMovement);
			moveCooldownTimer.reset();
		}
	}
}