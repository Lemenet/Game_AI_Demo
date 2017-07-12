#pragma once


//------------------------------------------------------------------------
//
//  Name:   SteeringBehaviors.h
//
//  Desc:   class to encapsulate steering behaviors for a Vehicle
//
//  Author: HuangMengdi 2017
//
//------------------------------------------------------------------------


#include <vector>
#include <windows.h>
#include <list>
#include <string>

#include "2D/Vector2D.h"
#include "Constants.h"
#include "Path.h"
#include "ParamLoader.h"

class Vehicle;
class CController;
class Wall2D;
class BaseGameEntity;

//---------------------- Constants ----------------------

//The radius fot the constraining circle for the wander behavior
const double WanderRad = 1.2;
//distance the wander circle is projected in front of the agent
const double WanderDist = 2.0;
//the maximum amount of displacement along the circle each frame
const double WanderJitterPerSec = 80.0;

//used in path following
const double WaypointSeekDist = 20;

//-------------------------------------------------------


class SteeringBehavior
{

private:
	enum summing_method{weighted_average, prioritized, dithered};

private:

	enum behavior_type
	{
		none = 0x00000,
		seek = 0x00002,
		flee = 0x00004,
		arrive = 0x00008,
		wander = 0x00010,
		cohesion = 0x00020,
		separation = 0x00040,
		allignment = 0x00080,
		obstacle_avoidance = 0x00100,
		wall_avoidance = 0x00200,
		follow_path = 0x00400,
		pursuit = 0x00800,
		evade = 0x01000,
		interpose = 0x02000,
		hide = 0x04000,
		flock = 0x08000,
		offset_pursuit = 0x10000,
	};

private:

	//The pointer of the owner vehicle
	Vehicle* pVehicle_;

	//the steering force created by the combined effect of all the selected behaviors
	Vector2D vSteeringForce_;

	//the current target
	Vector2D currentTarget_;

	//these can be used to keep track of friends, pursuers, or prey
	Vehicle* pTargetAgent1_;
	Vehicle* pTargetAgent2_;

	//the current target
	Vector2D    m_vTarget;

	//length of the 'detection box' utilized in obstacle avoidance
	double                 m_dDBoxLength;


	//a vertex buffer to contain the feelers rqd for wall avoidance  
	std::vector<Vector2D> m_Feelers;

	//the length of the 'feeler/s' used in wall detection
	double                 m_dWallDetectionFeelerLength;



	//the current position on the wander circle the agent is
	//attempting to steer towards
	Vector2D     m_vWanderTarget;

	//explained above
	double        m_dWanderJitter;
	double        m_dWanderRadius;
	double        m_dWanderDistance;


	//multipliers. These can be adjusted to effect strength of the  
	//appropriate behavior. Useful to get flocking the way you require
	//for example.
	double       dWeightSeparation_;
	double       dWeightCohesion_;
	double       dWeightAlignment_;
	double       dWeightWander_;
	double       dWeightObstacleAvoidance_;
	double       dWeightWallAvoidance_;
	double       dWeightSeek_;
	double       dWeightFlee_;
	double       dWeightArrive_;
	double       dWeightPursuit_;
	double       dWeightOffsetPursuit_;
	double       dWeightInterpose_;
	double       dWeightHide_;
	double       dWeightEvade_;
	double       dWeightFollowPath_;

	//how far the agent can 'see'
	double        m_dViewDistance;

	//pointer to any current path
	Path*         pPath_;

	//the distance (squared) a vehicle has to be from a path waypoint before
	//it starts seeking to the next waypoint
	double        m_dWaypointSeekDistSq;


	//any offset used for formations or offset pursuit
	Vector2D     m_vOffset;



	//binary flags to indicate whether or not a behavior should be active
	int           m_iFlags;


	//Arrive makes use of these to determine how quickly a vehicle
	//should decelerate to its target
	enum Deceleration { slow = 3, normal = 2, fast = 1 };

	//default
	Deceleration m_Deceleration;

	//is cell space partitioning to be used or not?
	bool          m_bCellSpaceOn;

	//what type of method is used to sum any active behavior
	summing_method  m_SummingMethod;


	//this function tests if a specific bit of m_iFlags is set
	bool      On(behavior_type bt) { return (m_iFlags & bt) == bt; }

	bool      AccumulateForce(Vector2D &sf, Vector2D ForceToAdd);

	//creates the antenna utilized by the wall avoidance behavior
	void      CreateFeelers();



	/* .......................................................

	BEGIN BEHAVIOR DECLARATIONS

	.......................................................*/


	//This behavior moves the agent towards a target position
	Vector2D Seek(Vector2D TargetPos);

	//This behavior returns a vector that moves the agent away
	//from a target position
	Vector2D Flee(Vector2D TargetPos);

	//This behavior is similar to seek but it attempts to arrive 
	//At the target position with a zero velocity
	Vector2D Arrive(Vector2D TargetPos, Deceleration deceleration);



	/* .......................................................

	END BEHAVIOR DECLARATIONS

	.......................................................*/

	//calculates and sums the steering forces from any active behaviors
	Vector2D CalculateWeightedSum();
	Vector2D CalculatePrioritized();
	Vector2D CalculateDithered();

	//helper method for Hide. Returns a position located on the other
	//side of an obstacle to the pursuer
	Vector2D GetHidingPosition(const Vector2D& posOb,
		const double     radiusOb,
		const Vector2D& posHunter);






public:

	SteeringBehavior(Vehicle* agent);

	virtual ~SteeringBehavior();

	//calculates and sums the steering forces from any active behaviors
	Vector2D Calculate();

	//calculates the component of the steering force that is parallel
	//with the vehicle heading
	double    ForwardComponent();

	//calculates the component of the steering force that is perpendicuar
	//with the vehicle heading
	double    SideComponent();



	//renders visual aids and info for seeing how each behavior is
	//calculated
	void      RenderAids();

	void      SetTarget(const Vector2D t) { m_vTarget = t; }

	void      SetTargetAgent1(Vehicle* Agent) { pTargetAgent1_ = Agent; }
	void      SetTargetAgent2(Vehicle* Agent) { pTargetAgent2_ = Agent; }

	void      SetOffset(const Vector2D offset) { m_vOffset = offset; }
	Vector2D  GetOffset()const { return m_vOffset; }

	void      SetPath(std::list<Vector2D> new_path) { pPath_->Set(new_path); }
	void      CreateRandomPath(int num_waypoints, int mx, int my, int cx, int cy)const
	{
		pPath_->CreateRandomPath(num_waypoints, mx, my, cx, cy);
	}

	Vector2D Force()const { return vSteeringForce_; }

	void      ToggleSpacePartitioningOnOff() { m_bCellSpaceOn = !m_bCellSpaceOn; }
	bool      isSpacePartitioningOn()const { return m_bCellSpaceOn; }

	void      SetSummingMethod(summing_method sm) { m_SummingMethod = sm; }


	void FleeOn() { m_iFlags |= flee; }
	void SeekOn() { m_iFlags |= seek; }
	void ArriveOn() { m_iFlags |= arrive; }
	void WanderOn() { m_iFlags |= wander; }
	void PursuitOn(Vehicle* v) { m_iFlags |= pursuit; pTargetAgent1_ = v; }
	void EvadeOn(Vehicle* v) { m_iFlags |= evade; pTargetAgent1_ = v; }
	void CohesionOn() { m_iFlags |= cohesion; }
	void SeparationOn() { m_iFlags |= separation; }
	void AlignmentOn() { m_iFlags |= allignment; }
	void ObstacleAvoidanceOn() { m_iFlags |= obstacle_avoidance; }
	void WallAvoidanceOn() { m_iFlags |= wall_avoidance; }
	void FollowPathOn() { m_iFlags |= follow_path; }
	void InterposeOn(Vehicle* v1, Vehicle* v2) { m_iFlags |= interpose; pTargetAgent1_ = v1; pTargetAgent2_ = v2; }
	void HideOn(Vehicle* v) { m_iFlags |= hide; pTargetAgent1_ = v; }
	void OffsetPursuitOn(Vehicle* v1, const Vector2D offset) { m_iFlags |= offset_pursuit; m_vOffset = offset; pTargetAgent1_ = v1; }
	void FlockingOn() { CohesionOn(); AlignmentOn(); SeparationOn(); WanderOn(); }

	void FleeOff() { if (On(flee))   m_iFlags ^= flee; }
	void SeekOff() { if (On(seek))   m_iFlags ^= seek; }
	void ArriveOff() { if (On(arrive)) m_iFlags ^= arrive; }
	void WanderOff() { if (On(wander)) m_iFlags ^= wander; }
	void PursuitOff() { if (On(pursuit)) m_iFlags ^= pursuit; }
	void EvadeOff() { if (On(evade)) m_iFlags ^= evade; }
	void CohesionOff() { if (On(cohesion)) m_iFlags ^= cohesion; }
	void SeparationOff() { if (On(separation)) m_iFlags ^= separation; }
	void AlignmentOff() { if (On(allignment)) m_iFlags ^= allignment; }
	void ObstacleAvoidanceOff() { if (On(obstacle_avoidance)) m_iFlags ^= obstacle_avoidance; }
	void WallAvoidanceOff() { if (On(wall_avoidance)) m_iFlags ^= wall_avoidance; }
	void FollowPathOff() { if (On(follow_path)) m_iFlags ^= follow_path; }
	void InterposeOff() { if (On(interpose)) m_iFlags ^= interpose; }
	void HideOff() { if (On(hide)) m_iFlags ^= hide; }
	void OffsetPursuitOff() { if (On(offset_pursuit)) m_iFlags ^= offset_pursuit; }
	void FlockingOff() { CohesionOff(); AlignmentOff(); SeparationOff(); WanderOff(); }

	bool isFleeOn() { return On(flee); }
	bool isSeekOn() { return On(seek); }
	bool isArriveOn() { return On(arrive); }
	bool isWanderOn() { return On(wander); }
	bool isPursuitOn() { return On(pursuit); }
	bool isEvadeOn() { return On(evade); }
	bool isCohesionOn() { return On(cohesion); }
	bool isSeparationOn() { return On(separation); }
	bool isAlignmentOn() { return On(allignment); }
	bool isObstacleAvoidanceOn() { return On(obstacle_avoidance); }
	bool isWallAvoidanceOn() { return On(wall_avoidance); }
	bool isFollowPathOn() { return On(follow_path); }
	bool isInterposeOn() { return On(interpose); }
	bool isHideOn() { return On(hide); }
	bool isOffsetPursuitOn() { return On(offset_pursuit); }

	double DBoxLength()const { return m_dDBoxLength; }
	const std::vector<Vector2D>& GetFeelers()const { return m_Feelers; }

	double WanderJitter()const { return m_dWanderJitter; }
	double WanderDistance()const { return m_dWanderDistance; }
	double WanderRadius()const { return m_dWanderRadius; }

	double SeparationWeight()const { return dWeightSeparation_; }
	double AlignmentWeight()const { return dWeightAlignment_; }
	double CohesionWeight()const { return dWeightCohesion_; }

};