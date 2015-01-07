/* ------------------------------------------------------------------------------
** _________   _________      ________    _____      _____  ___________ _________
** \_   ___ \ /   _____/     /  _____/   /  _  \    /     \ \_   _____//   _____/
** /    \  \/ \_____  \     /   \  ___  /  /_\  \  /  \ /  \ |    __)_ \_____  \ 
** \     \____/        \    \    \_\  \/    |    \/    Y    \|        \/        \
**  \______  /_______  /     \______  /\____|__  /\____|__  /_______  /_______  /
**        \/        \/             \/         \/         \/        \/        \/ 
**
** World.cpp
** Implementation of the World
**
** Author: Samuel-Ricardo Carriere
** ------------------------------------------------------------------------------*/

#include "stdafx.h"

#include "World.h"

#include <OgreOverlayManager.h>

World::World()
{
	TANK_MESH_NAME = "Tank.mesh";
	MINE_MESH_NAME = "Mine.mesh";
	MISSILE_MESH_NAME = "Missile.mesh";

	this->sceneManager = NULL;
	teamCount = 0;

	for(int i = 0; i < MAX_CHARACTER_PER_TEAM; ++i)
	{
		teams[i] = NULL;
	}
}

World::~World()
{
	for(int i = 0; i < MAX_CHARACTER_PER_TEAM; ++i)
	{
		if(teams[i])
		{
			delete teams[i];
		}
	}
}

void World::init(Ogre::SceneManager* sceneManager)
{
	this->sceneManager = sceneManager;
}

void World::createScene()
{
	// Set the scene's ambient light
	sceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

	// Create a Light and set its position
	Ogre::Light* light = sceneManager->createLight("MainLight");
	light->setPosition(Ogre::Real(-MAP_TILE_SIZE), 80.0, Ogre::Real(-MAP_TILE_SIZE));

	light = sceneManager->createLight("MainLight2");
	light->setPosition(Ogre::Real(-MAP_TILE_SIZE), 80.0, Ogre::Real(9*MAP_TILE_SIZE));

	light = sceneManager->createLight("MainLight3");
	light->setPosition(Ogre::Real(9*MAP_TILE_SIZE), 80.0f, Ogre::Real(-MAP_TILE_SIZE));

	light = sceneManager->createLight("MainLight4");
	light->setPosition(Ogre::Real(9*MAP_TILE_SIZE), 80.0f, Ogre::Real(9*MAP_TILE_SIZE));

	sceneManager->setSkyDome(true, "Examples/CloudySky", 5, 8);

	// create a floor mesh resource
	Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
												  Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), Ogre::Real(8*MAP_TILE_SIZE), Ogre::Real(8*MAP_TILE_SIZE),
												  10, 10, true, 1, 10, 10, Ogre::Vector3::UNIT_Z);

	// create a floor entity, give it a material, and place it at the origin
	Ogre::SceneNode* floorNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	Ogre::Entity* floor = sceneManager->createEntity("Floor", "floor");
	floor->setMaterialName("SceneMaterial/FloorSand");
	floor->setCastShadows(false);
	floorNode->attachObject(floor);
	floorNode->setPosition(87.5, 0.0, 87.5);

	//TODO: put this in function
	//Delimiter crate creation
	Ogre::Vector3 scaleVector(2.5, 2.5, 2.5);
	Ogre::SceneNode* crateNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	Ogre::Entity* crate = sceneManager->createEntity("crate1", "WoodCrate.mesh");
	crateNode->attachObject(crate);
	crateNode->setScale(scaleVector);
	crateNode->setPosition(Ogre::Real(4*MAP_TILE_SIZE), 12.5, 0.0);

	crateNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	crate = sceneManager->createEntity("crate2", "WoodCrate.mesh");
	crateNode->attachObject(crate);
	crateNode->setScale(scaleVector);
	crateNode->setPosition(Ogre::Real(7*MAP_TILE_SIZE), 12.5, Ogre::Real(4*MAP_TILE_SIZE));

	crateNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	crate = sceneManager->createEntity("crate3", "WoodCrate.mesh");
	crateNode->attachObject(crate);
	crateNode->setScale(scaleVector);
	crateNode->setPosition(0.0, 12.5, Ogre::Real(4*MAP_TILE_SIZE));

	crateNode = sceneManager->getRootSceneNode()->createChildSceneNode();
	crate = sceneManager->createEntity("crate4", "WoodCrate.mesh");
	crateNode->attachObject(crate);
	crateNode->setScale(scaleVector);
	crateNode->setPosition(Ogre::Real(4*MAP_TILE_SIZE), 12.5, Ogre::Real(7*MAP_TILE_SIZE));

	labelOverlay = Ogre::OverlayManager::getSingleton().create("labelOverlay");
}

void World::addTeam(int teamId, std::string teamName, std::string characterNames[MAX_CHARACTER_PER_TEAM])
{
	Team* team = new Team(teamName, teamId);
	teams[teamCount++] = team;

	for(int i = 0; i < MAX_CHARACTER_PER_TEAM; ++i)
	{
		Ogre::SceneNode* bodyNode = sceneManager->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* entity = sceneManager->createEntity(characterNames[i], TANK_MESH_NAME);
		bodyNode->attachObject(entity);

		std::string mineName = "mine_" + characterNames[i];
		Ogre::SceneNode* mineNode = sceneManager->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* mineEntity = sceneManager->createEntity(mineName, MINE_MESH_NAME);
		mineNode->attachObject(mineEntity);
		mineNode->setScale(Ogre::Vector3(2.0, 2.0, 2.0));
		mineNode->setVisible(false);

		std::string missileName = "missile_" + characterNames[i];
		Ogre::SceneNode* missileNode = sceneManager->getRootSceneNode()->createChildSceneNode();
		Ogre::Entity* missileEntity = sceneManager->createEntity(missileName, MISSILE_MESH_NAME);
		missileNode->attachObject(missileEntity);
		missileNode->setVisible(false);

		Mine* mine = new Mine(mineNode, mineName);
		Missile* missile = new Missile(missileNode, missileName, teamId, i);
		TextOverlay* nameOverlay = new TextOverlay(labelOverlay, characterNames[i], characterNames[i], bodyNode, sceneManager->getCamera("PlayerCam")->getViewport());
		TextOverlay* lifeOverlay = new TextOverlay(labelOverlay, "Life : 3", "Life_" + characterNames[i], bodyNode, sceneManager->getCamera("PlayerCam")->getViewport(), Ogre::Vector3::UNIT_Y*15.0);
		lifeOverlay->setColors(Ogre::ColourValue(1.0, 0.5, 0.5), Ogre::ColourValue(1.0, 1.0, 1.0));

		Character* character = new Character(bodyNode, mine, missile, nameOverlay, lifeOverlay, characterNames[i], teamId, i);
		team->addCharacter(character);
	}

	if(teamCount == MAX_TEAM)
	{
		gameStart();
		sendAllPosition();
	}
}

void World::removeTeam(int teamId)
{
	Team* team = getTeam(teamId);
	if(team)
	{
		delete team;
	}
}

Team* World::getTeam(int teamId)
{
	for(int i = 0; i < MAX_TEAM; ++i)
	{
		if(teams[i] && teams[i]->getId() == teamId)
		{
			return teams[i];
		}
	}
	return NULL;
}

void World::addTime(Ogre::Real deltaTime)
{
	for(int i = 0; i < MAX_TEAM; ++i)
	{
		if(teams[i])
		{
			teams[i]->addTime(deltaTime, sceneManager->getCamera("PlayerCam"));
		}
	}
}

void World::gameStart()
{
	char numstr[21]; // Enough to hold all numbers up to 64-bits
	sprintf(numstr, "%d", MAX_TEAM);
	std::string message = "Game:GameStart:";
	message += numstr;
	sprintf(numstr, "%d", MAX_CHARACTER_PER_TEAM);
	message += ":";
	message += numstr;

	for(int i = 0; i < MAX_TEAM; ++i)
	{
		sprintf(numstr, "%d", teams[i]->getId());
		message += ":";
		message += numstr;
	}
	QueueController::getInstance().addMessage(message);
}

void World::sendAllPosition()
{
	for(int teamIndex = 0; teamIndex < MAX_TEAM; ++teamIndex)
	{
		for(int characterIndex = 0; characterIndex < MAX_CHARACTER_PER_TEAM; ++characterIndex)
		{
			teams[teamIndex]->getCharacter(characterIndex)->sendPosition();
		}
	}
}

void World::mineHit(int hitPlayerId, int hitCharacterId, int originPlayerId, int originCharacterId)
{
	//TODO: refactor with the life system
	getTeam(originPlayerId)->getCharacter(originCharacterId)->getMine()->setVisible(false);
}

void World::missileHitCharacter(int hitPlayerId, int hitCharacterId, int originPlayerId, int originCharacterId)
{
	//TODO: refactor with the life system
	getTeam(originPlayerId)->getCharacter(originCharacterId)->getMissile()->setVisible(false);
}

void World::missileHitMine(int hitPlayerId, int hitCharacterId, int originPlayerId, int originCharacterId)
{
	//TODO: refactor with the life system
	getTeam(originPlayerId)->getCharacter(originCharacterId)->getMissile()->setVisible(false);
	getTeam(hitPlayerId)->getCharacter(hitCharacterId)->getMine()->setVisible(false);
}

void World::missileHitMissile(int hitPlayerId, int hitCharacterId, int originPlayerId, int originCharacterId)
{
	//TODO: refactor with the life system
	getTeam(originPlayerId)->getCharacter(originCharacterId)->getMissile()->setVisible(false);
	getTeam(hitPlayerId)->getCharacter(hitCharacterId)->getMissile()->setVisible(false);
}