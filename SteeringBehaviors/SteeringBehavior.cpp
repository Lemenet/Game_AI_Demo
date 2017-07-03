#include <cassert>

#include "Vehicle.h"
#include "SteeringBehavior.h"





/////////////////////////////////////////////////////////////////////////////// START OF BEHAVIORS

//------------------------------- Seek -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  direct the agent towards the target
//------------------------------------------------------------------------
Vector2D SteeringBehavior::Seek(Vector2D TargetPos)
{
	Vector2D desiredVector = Vec2DNormalize(TargetPos - pOwnerVehicle_->GetPosition()) * pOwnerVehicle_->GetMaxSpeed();
	return (pOwnerVehicle_->GetVelocity() - desiredVector);
}

//----------------------------- Flee -------------------------------------
//
//  Does the opposite of Seek
//------------------------------------------------------------------------
Vector2D SteeringBehavior::Flee(Vector2D TargetPos)
{
	const double panicDistanceSq = 100.0 * 100.0;
	if (Vec2DDistanceSq(pOwnerVehicle_->GetPosition(), TargetPos) > panicDistanceSq)
	{
		return Vector2D(0, 0);
	}

	Vector2D desiredVector = Vec2DNormalize(pOwnerVehicle_->GetPosition() - TargetPos) * pOwnerVehicle_->GetMaxSpeed();
	return (desiredVector - pOwnerVehicle_->GetVelocity());
}