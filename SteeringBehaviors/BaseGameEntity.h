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

namespace EEntityType
{
	enum type
	{
		default = -1,
	};
}

class BaseGameEntity
{
public:

	

private:

	//Each entity must have a unique ID
	int ID_;

	//Every entity has a type associated with it (health, troll, ammo etc)
	int entityType_;

	//this is a generic flag
	bool bTag_;

	//used by the constructor to give each entity a unique ID
	int NextVelidID()
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
	double boundingRadius_;


protected:
	//------Constroctor-------

	BaseGameEntity() :
		ID_(NextVelidID()),
		boundingRadius_(0.0),
		position_(Vector2D()),
		scale_(Vector2D(1.0, 1.0)),
		entityType_(EEntityType::default),
		bTag_(false)
	{}

	BaseGameEntity(int entityType) :
		ID_(NextVelidID()),
		boundingRadius_(0.0),
		position_(Vector2D()),
		scale_(Vector2D(1.0, 1.0)),
		entityType_(entityType),
		bTag_(false)
	{}

	BaseGameEntity(int entityType, Vector2D pos, double r) :
		ID_(NextVelidID()),
		boundingRadius_(r),
		position_(pos),
		scale_(Vector2D(1.0, 1.0)),
		entityType_(entityType),
		bTag_(false)
	{}


};