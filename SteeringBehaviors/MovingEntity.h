#pragma once

//------------------------------------------------------------------------
//
//  Name:   MovingEntity.h
//
//  Desc:   A base class defining an entity that moves. The entity has 
//          a local coordinate system and members for defining its
//          mass and velocity.
//
//  Author: HuangMengdi 2017
//
//------------------------------------------------------------------------

#include <cassert>

#include "2D/Vector2D.h"
#include "BaseGameEntity.h"


class MovingEntity : public BaseGameEntity
{
public:
	MovingEntity(
		Vector2D position,
		double radius,
		Vector2D velocity,
		double max_speed,
		Vector2D heading,
		double mass,
		Vector2D scale,
		double turn_rate,
		double max_force
	) :
		BaseGameEntity(0,position,radius),
		headDirection_(heading),
		sideDirection_(heading.Perp()),
		velocity_(velocity),
		dMass_(mass),
		dMaxSpeed_(max_speed),
		dMaxForce_(max_force),
		dMaxTurnRate_(turn_rate)
	{
		scale_ = scale;
	}

	~MovingEntity() {}


	//----------Accessors----------

	//Velocity
	Vector2D GetVelocity()const { return velocity_; }
	void SetVelocity(const Vector2D& NewVel) { velocity_ = NewVel; }

	//Mass
	double GetMass()const { return dMass_; }
	
	//Side
	Vector2D GetSide()const { return sideDirection_; }

	//Max speed
	double GetMaxSpeed()const { return dMaxSpeed_; }
	void SetMaxSpeed(double new_speed) { dMaxSpeed_ = new_speed; }

	//Max force
	double GetMaxForce()const { return dMaxForce_; }
	void SetMaxForce(double mf) { dMaxForce_ = mf; }

	//Current speed, based on the velocity length
	bool IsSpeedMaxedOut()const { return dMaxSpeed_*dMaxSpeed_ >= velocity_.LengthSq(); }
	double GetSpeed()const { return velocity_.Length(); }
	double GetSpeedSq()const { return velocity_.LengthSq(); }

	Vector2D GetHeadDirection()const { return headDirection_; }
	void SetHeadDirection(Vector2D new_heading);
	bool RotateHeadDirectionToFacePosition(Vector2D target);

	double GetMaxTurnRate()const { return dMaxTurnRate_; }
	void SetMaxTurnRate(double val) { dMaxTurnRate_ = val; }


protected:

	//The velocity of the entity
	Vector2D velocity_;

	//A normalized vector pointing in the direction the entity is heading. 
	Vector2D headDirection_;

	//A vector perpendicular to the heading vector
	Vector2D sideDirection_;

	//The mass of the entity, (m) for calculate force
	double dMass_;

	//The Maximum speed of the entity may travel at
	double dMaxSpeed_;

	//The maximum force this entity can produce to power itself  
	//(think rockets and thrust)
	double dMaxForce_;

	//the maximum rate (radians per second)this vehicle can rotate         
	double dMaxTurnRate_;



private:

};



//------------------------- SetHeading ----------------------------------------
//
//  first checks that the given heading is not a vector of zero length. If the
//  new heading is valid this fumction sets the entity's heading and side 
//  vectors accordingly
//-----------------------------------------------------------------------------
void MovingEntity::SetHeadDirection(Vector2D new_heading)
{
	//Ensure the head direction always be a normalized Vector2D
	assert(new_heading.LengthSq() <= 1.00001);


	headDirection_ = new_heading;

	//The side vector must always be perpendicular to the heading
	sideDirection_ = new_heading.Perp();
}



//--------------------------- RotateHeadingToFacePosition ---------------------
//
//  given a target position, this method rotates the entity's heading and
//  side vectors by an amount not greater than m_dMaxTurnRate until it
//  directly faces the target.
//
//  returns true when the heading is facing in the desired direction
//-----------------------------------------------------------------------------
bool MovingEntity::RotateHeadDirectionToFacePosition(Vector2D target)
{
	Vector2D toTarget = Vec2DNormalize(target - position_);

	//first determine the angle between the heading vector and the target
	double angle = acos(headDirection_.Dot(toTarget));

	//return true if the player is facing the target
	if (angle < 0.00001)
	{
		return true;
	}

	//clamp the amount to turn to the max turn rate
	if (angle > dMaxTurnRate_)
	{
		angle = dMaxTurnRate_;
	}

	//The next few lines use a rotation matrix to rotate the player's heading
	//vector accordingly
	C2DMatrix RotationMatrix;

	//notice how the direction of rotation has to be determined when creating
	//the rotation matrix
	RotationMatrix.Rotate(angle * headDirection_.Sign(toTarget));
	RotationMatrix.TransformVector2Ds(headDirection_);
	RotationMatrix.TransformVector2Ds(velocity_);

	//finally recreate m_vSide
	sideDirection_ = headDirection_.Perp();

	return false;
}
