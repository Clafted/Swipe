#include "../Headers/Player.h"
#include "../Headers/SoundManager.h"

/*Override of Object::moveTarget(). Plays a sound and does not move the target if the coordinates are outside of the map.*/
void Player::moveTarget(float x, float y)
{
	// Do not move the target if the coordinates are outside of the map
	if (((x + xPos) / map->tileWidth) + (map->width / 2.0f) >= map->width || ((x + xPos) / map->tileWidth) + (map->width / 2.0f) < 0) return;
	if (((y + yPos) / map->tileWidth) + (map->width / 2.0f) >= map->width || ((y + yPos) / map->tileWidth) + (map->width / 2.0f) < 0) return;
	
	SoundManager::Instance()->soundEngine->play2D("SFX/PlayerMove.wav", false); // Play a sound
	Object::moveTarget(x, y); // Move the target
}

void Player::update(float updatePeriod)
{
	Object::update(updatePeriod);
	damageCooldownTimer.updateClock(updatePeriod);
}

void Player::damage(int dmg)
{
	if (damageCooldownTimer.numberOfTicksPassed() > 0)
	{
		lives -= dmg;
		damageCooldownTimer.reset();
	}
}

void Player::handleCollisionWith(Collider& other)
{
	if (other.contextObject.classID == "Ram")
	{
		damage(1);
	}
	else if (other.contextObject.classID == "ManHole")
	{
		damage(2);
	}
	else
	{
		std::cout << "Random Player collision";
	}
}