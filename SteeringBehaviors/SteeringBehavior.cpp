#include <cassert>

#include "Vehicle.h"
#include "SteeringBehavior.h"

using std::string;
using std::vector;


//------------------------- ctor -----------------------------------------
//
//------------------------------------------------------------------------
SteeringBehavior::SteeringBehavior(Vehicle* agent) :


	pOwnerVehicle_(agent),
	m_iFlags(0),
	m_dDBoxLength(Prm.MinDetectionBoxLength),
	m_dWeightCohesion(Prm.CohesionWeight),
	m_dWeightAlignment(Prm.AlignmentWeight),
	m_dWeightSeparation(Prm.SeparationWeight),
	m_dWeightObstacleAvoidance(Prm.ObstacleAvoidanceWeight),
	m_dWeightWander(Prm.WanderWeight),
	m_dWeightWallAvoidance(Prm.WallAvoidanceWeight),
	m_dViewDistance(Prm.ViewDistance),
	m_dWallDetectionFeelerLength(Prm.WallDetectionFeelerLength),
	m_Feelers(3),
	m_Deceleration(normal),
	m_pTargetAgent1(NULL),
	m_pTargetAgent2(NULL),
	m_dWanderDistance(WanderDist),
	m_dWanderJitter(WanderJitterPerSec),
	m_dWanderRadius(WanderRad),
	m_dWaypointSeekDistSq(WaypointSeekDist*WaypointSeekDist),
	m_dWeightSeek(Prm.SeekWeight),
	m_dWeightFlee(Prm.FleeWeight),
	m_dWeightArrive(Prm.ArriveWeight),
	m_dWeightPursuit(Prm.PursuitWeight),
	m_dWeightOffsetPursuit(Prm.OffsetPursuitWeight),
	m_dWeightInterpose(Prm.InterposeWeight),
	m_dWeightHide(Prm.HideWeight),
	m_dWeightEvade(Prm.EvadeWeight),
	m_dWeightFollowPath(Prm.FollowPathWeight),
	m_bCellSpaceOn(false),
	m_SummingMethod(prioritized)


{
	//stuff for the wander behavior
	double theta = RandFloat() * TwoPi;

	//create a vector to a target position on the wander circle
	m_vWanderTarget = Vector2D(m_dWanderRadius * cos(theta),
		m_dWanderRadius * sin(theta));

	//create a Path
	m_pPath = new Path();
	m_pPath->LoopOn();

}

//---------------------------------dtor ----------------------------------
SteeringBehavior::~SteeringBehavior() { delete m_pPath; }




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

Vector2D SteeringBehavior::Arrive(Vector2D TargetPos, Deceleration deceleration)
{
	Vector2D ToTarget = TargetPos - pOwnerVehicle_->GetPosition();
	//Calculate the distance to the target
	double dist = ToTarget.Length();

	if (dist > 0)
	{
		//because Deceleration is enumerated as an int, this value is required
		//to provide fine tweaking of the deceleration..
		const double DecelerationTweaker = 0.3;

		//Calculate the speed required to riach the target given the desired deceleration
		double speed = dist / ((double)deceleration * DecelerationTweaker);

		//Make sure the velocity does not exceed the max
		speed = min(speed, pOwnerVehicle_->GetMaxSpeed());

		//From here proceed just like Seek except we don't need to normalize
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: distance.
		Vector2D DesiredVelocity = ToTarget*speed / dist;

		return (DesiredVelocity - pOwnerVehicle_->GetVelocity());
	}

	return Vector2D(0, 0);

}
