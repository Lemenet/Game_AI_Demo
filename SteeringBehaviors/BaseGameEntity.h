#pragma once

//------------------------------------------------------------------------
//
//  Name: BaseGameEntity.h
//
//  Desc: Base class to define a common interface for all game
//        entities
//
//  Author: HaungMengdi 2017
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

	//this can be used to create an entity with a 'forced' ID. It can be used
	//when a previously created entity has been removed and deleted from the
	//game for some reason. For example, The Raven map editor uses this ctor 
	//in its undo/redo operations. 
	//USE WITH CAUTION!

	BaseGameEntity(int entityType, int ForcedID) :
		ID_(ForcedID),
		boundingRadius_(0.0),
		position_(Vector2D()),
		scale_(Vector2D(1.0, 1.0)),
		entityType_(entityType),
		bTag_(false)
	{}


public:

	virtual ~BaseGameEntity() {}

	virtual void Update(double deltaTime) {}

	virtual void Render() {}

	virtual bool HandleMessage(const Telegram& msg) { return false; }

	//Entity should be able to read and write their data to a stream
	virtual void WriteData(std::ostream& os) const {}
	virtual void ReadData(std::ifstream& is) {}


public:
	//----------Interface----------

	//position_
	Vector2D GetPosition() const { return position_; }
	void SetPosition(const Vector2D& newPosition) { position_ = newPosition; }

	//boundingRadius_
	double GetRadius() const { return boundingRadius_; }
	void SetRadius(double r) { boundingRadius_ = r; }
	
	//ID_
	int GetID() const { return ID_; }

	//bTag_
	bool IsTagged() const { return bTag_; }
	void DoTag() { bTag_ = true; }
	void UnTag() { bTag_ = false; }

	//Scale_
	Vector2D GetScale() const { return scale_; }
	void SetScale(const Vector2D& val)
	{
		boundingRadius_ *= MaxOf(val.x, val.y) / MaxOf(scale_.x, scale_.y);
		scale_ = val;
	}
	void SetScale(double val)
	{
		boundingRadius_ *= (val / MaxOf(scale_.x, scale_.y));
		scale_ = Vector2D(val, val);
	}

	//Entity type
	int GetEntityType() const { return entityType_; }
	void SetEntityType(int newType) { entityType_ = newType; }

};