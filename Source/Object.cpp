#include "../Headers/Object.h"

/*Move the target position of an Object by the given movement coordinates. Only sets gliding to true if target is not equal to current position.*/
void Object::moveTarget(float x, float y)
{
	// Change the target position if not already gliding
	if (!gliding && !(x == 0.0f && y == 0.0f))
	{
		gliding = true;

		xStarting = xPos;
		yStarting = yPos;

		xTarget = xStarting + x;
		yTarget = yStarting + y;

		xVelocity = speed * x / ((y != 0) ? y : abs(x));
		yVelocity = speed * y / ((x != 0) ? x : abs(y));
	}
}

void Object::glideToTarget()
{
	float distanceX = xTarget - xPos;
	float distanceY = yTarget - yPos;

	// Jump to target position if close enough already
	if (abs(distanceX) < abs(xVelocity * updatePeriod)) move(distanceX, 0.0f);
	if (abs(distanceY) < abs(yVelocity * updatePeriod)) move(0.0f, distanceY);

	// Stop gliding if target has been reached
	if (xPos == xTarget && yPos == yTarget)
	{
		gliding = false;
		return;
	}

	// Move towards the target
	move(xVelocity * updatePeriod, yVelocity * updatePeriod);

}