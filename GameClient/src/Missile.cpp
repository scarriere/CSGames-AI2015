/* ------------------------------------------------------------------------------
** _________   _________      ________    _____      _____  ___________ _________
** \_   ___ \ /   _____/     /  _____/   /  _  \    /     \ \_   _____//   _____/
** /    \  \/ \_____  \     /   \  ___  /  /_\  \  /  \ /  \ |    __)_ \_____  \ 
** \     \____/        \    \    \_\  \/    |    \/    Y    \|        \/        \
**  \______  /_______  /     \______  /\____|__  /\____|__  /_______  /_______  /
**        \/        \/             \/         \/         \/        \/        \/ 
**
** Missile.cpp
** Implementation of the Missile
**
** Author: Samuel-Ricardo Carriere
** ------------------------------------------------------------------------------*/

#include "stdafx.h"

#include "Missile.h"

Missile::Missile(Ogre::SceneNode* bodyNode, std::string name)
{
	// 3d Infos
	MISSILE_MESH_HEIGHT = 8.5;
	this->name = name;
	this->bodyNode = bodyNode;
	this->bodyNode->pitch(Ogre::Radian(Ogre::Degree(90)),Ogre::Node::TS_WORLD);

	this->position = Vector2(0, 0);
	this->targetPosition = this->position;
	this->subStepPosition = Ogre::Vector3::ZERO;
}

void Missile::addTime(Ogre::Real deltaTime)
{
	updateBody(deltaTime);
}

void Missile::init(Vector2 position, MapDirection::MapDirection direction)
{
	this->position = position;
	Ogre::Vector3 startingVector(Ogre::Real(position.x*MAP_TILE_SIZE), MISSILE_MESH_HEIGHT, Ogre::Real(position.y*MAP_TILE_SIZE));
	this->bodyNode->setPosition(startingVector);
	this->subStepPosition = startingVector;
	this->targetPosition = calculateTargetPosition(direction);
	this->direction = direction;
}

Vector2 Missile::calculateTargetPosition(MapDirection::MapDirection direction)
{
	if(direction == MapDirection::UP)
	{
		return Vector2(position.x, MAP_HEIGHT-1);
	}
	else if(direction == MapDirection::DOWN)
	{
		return Vector2(position.x, 0);
	}
	else if(direction == MapDirection::LEFT)
	{
		return Vector2(0, position.y);
	}
	else if(direction == MapDirection::RIGHT)
	{
		return Vector2(MAP_WIDTH-1, position.y);
	}
	else
	{
		return position;
	}
}

void Missile::launch()
{
	setVisible(true);
}


void Missile::updateBody(Ogre::Real deltaTime)
{
	if(bodyNode != NULL && isVisible())
	{
		Ogre::Vector3 currentPosition = bodyNode->getPosition();
		Ogre::Vector3 goalDirection = subStepPosition - currentPosition;
		
		if(goalDirection.length() < Ogre::Real(1.0))
		{
			if(position == targetPosition)
			{
				goalDirection = Ogre::Vector3::ZERO;
				// TODO: generate an missile reach map limit
			}
			else
			{
				Vector2 newPosition = calculateNextStep();

				if(newPosition == position)
				{
					goalDirection = Ogre::Vector3::ZERO;
				}
				else
				{
					Map::getInstance().moveMissileTile(position, newPosition);
					position = newPosition;

					/* TODO: send the missile position
					std::string message = NetUtility::generateMoveCharacterMessage(teamId, characterId, position.x, position.y);
					QueueController::getInstance().addMessage(message);
					*/

					subStepPosition = Ogre::Vector3(Ogre::Real(position.x*MAP_TILE_SIZE), currentPosition.y, Ogre::Real(position.y*MAP_TILE_SIZE));
					goalDirection = subStepPosition - currentPosition;
				}
			}
		}

		if(goalDirection != Ogre::Vector3::ZERO)
		{
			// Calculate direction
			goalDirection.normalise();
			Ogre::Quaternion currentDirection = bodyNode->getOrientation().yAxis().getRotationTo(goalDirection);

			// Find the rotation in yaw
			Ogre::Real yawToGoal = currentDirection.getYaw().valueDegrees();

			bodyNode->yaw(Ogre::Degree(yawToGoal), Ogre::Node::TS_WORLD);

			bodyNode->translate(0, deltaTime * MISSILE_WALK_SPEED, 0, Ogre::Node::TS_LOCAL);
		}
	}
}

void Missile::sendPosition()
{
	//TODO make a missile update message
	/*
	std::string message = NetUtility::generateMoveCharacterMessage(teamId, characterId, position.x, position.y);
	QueueController::getInstance().addMessage(message);
	*/
}

void Missile::setVisible(bool visible)
{
	bodyNode->setVisible(visible);
}

bool Missile::isVisible()
{
	return bodyNode->getAttachedObject(name)->isVisible();
}

Vector2 Missile::calculateNextStep()
{
	if(position.x > targetPosition.x)
	{
		return Vector2(position.x-1, position.y);
	}
	else if(position.x < targetPosition.x)
	{
		return Vector2(position.x+1, position.y);
	}
	else if(position.y > targetPosition.y)
	{
		return Vector2(position.x, position.y-1);
	}
	else if(position.y < targetPosition.y)
	{
		return Vector2(position.x, position.y+1);
	}
	return position;
}

MapDirection::MapDirection Missile::getDirection()
{
	return direction;
}
