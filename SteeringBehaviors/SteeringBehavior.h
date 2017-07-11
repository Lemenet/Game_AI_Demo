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
//#include "ParamLoader.h"

class Vehicle;
class CController;
class Wall2D;
class BaseGameEntity;

//---------------------- Constants ----------------------

//The radius fot the constraining circle for the wander behavior
double WanderRadius = 1.2;
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

private:

	//The pointer of the owner vehicle
	Vehicle* pOwnerVehicle_;

	//the steering force created by the combined effect of all the selected behaviors
	Vector2D steeringForce_;

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
	double        dViewDistance_;

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







public:

	SteeringBehavior(Vehicle* agent);

	virtual ~SteeringBehavior();


};