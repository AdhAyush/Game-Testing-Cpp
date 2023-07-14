
#include "Arrow.h"

using namespace sf;

// The constructor
Arrow::Arrow()
{
	m_ArrowTexture.loadFromFile("graphics/arow.png");
	m_ArrowShape.setTexture(m_ArrowTexture);
	m_ArrowShape.setOrigin(25, 25);

}

void Arrow::shoot(float startX, float startY,
	float targetX, float targetY)
{
	// Keep track of the Arrow
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	// Calculate the gradient of the flight path
	float gradient = (startX - targetX) / (startY - targetY);

	// Any gradient less than zero needs to be negative
	if (gradient < 0)
	{
		gradient *= -1;
	}

	// Calculate the ratio between x and t
	float ratioXY = m_ArrowSpeed / (1 + gradient);

	// Set the "speed" horizontally and vertically
	m_ArrowDistanceY = ratioXY;
	m_ArrowDistanceX = ratioXY * gradient;

	// Point the Arrow in the right direction
	if (targetX < startX)
	{
		m_ArrowDistanceX *= -1;
	}

	if (targetY < startY)
	{
		m_ArrowDistanceY *= -1;
	}

	// Finally, assign the results to the
	// member variables
	//m_XTarget = targetX;
	//m_YTarget = targetY;

	// Set a max range of 1000 pixels
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	// Calculate the angle the player is facing
	float angle = (atan2(targetY - startY,
		targetX - startX)
		* 180) / 3.141;


	// Position the Arrow ready to be drawn
	m_ArrowShape.setPosition(m_Position);

	//set rotation towards the target
	m_ArrowShape.setRotation(angle);
}

void Arrow::stop()
{
	m_InFlight = false;
}

bool Arrow::isInFlight()
{
	return m_InFlight;
}

FloatRect Arrow::getPosition()
{
	return m_ArrowShape.getGlobalBounds();
}

Sprite Arrow::getSprite()
{
	return m_ArrowShape;
}


void Arrow::update(float elapsedTime)
{
	// Update the Arrow position variables
	m_Position.x += m_ArrowDistanceX * elapsedTime;
	m_Position.y += m_ArrowDistanceY * elapsedTime;

	// Move the Arrow
	m_ArrowShape.setPosition(m_Position);

	

	// Has the Arrow gone out of range?
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}

}