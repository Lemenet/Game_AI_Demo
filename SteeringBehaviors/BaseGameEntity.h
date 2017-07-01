#pragma once

//------------------------------------------------------------------------
//
//  Name: BaseGameEntity.h
//
//  Desc: Base class to define a common interface for all game
//        entities
//
//  Author: haungmegndi 2017
//
//------------------------------------------------------------------------

#include <vector>
#include <string>
#include "2D/Vector2D.h"
#include "2D/geometry.h"
#include "misc/utils.h"

struct Telegram;


class BaseGameEntity
{
public:

	enum { default_entity_type = -1 };

private:

	//Each entity must have a unique ID
	int ID_;

	//Every entity has a type associated with it (health, troll, ammo etc)
	int entityType_;

	//this is a generic flag
	bool bTag_;

	//used by the constructor to give each entity a unique ID
	inline int NextVelidID()
	{
		static int NextID = 0;
		return NextID++;
	}

protected:

	//Its location in the environment
	Vector2D position_;

	//Its scale for the entity shape
	Vector2D scale_;

	//The length of this object's bounding radius
	double boundingRadius;


protected:
	//------Constroctor-------

	BaseGameEntity();

	BaseGameEntity(int entityType);


};