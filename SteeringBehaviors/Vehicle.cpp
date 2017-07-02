#include "Vehicle.h"
#include "2D/C2DMatrix.h"
#include "2D/geometry.h"
//#include "SteeringBehavior"
#include "2D/Transformations.h"
//#include "GameWorld.h"
#include "misc/CellSpacePartition.h"
#include "misc/Cgdi.h"

Vehicle::Vehicle(GameWorld * world, Vector2D position, double rotation, Vector2D velocity, double mass, double max_force, double max_speed, double max_turn_rate, double scale) : 
	MovingEntity(position, scale, velocity, max_speed, Vector2D(sin(rotation), -cos(rotation)), mass, Vector2D(scale, scale), max_turn_rate, max_force),
	pOwnedWorld_(world),
	smoothedHeading_(Vector2D(0, 0)),
	bSmoothingOn_(false),
	dTimeElapsed_(0.0)
{
	InitializeBuffer();

	//set up the steering behavior class
	pSteering_ = new SteeringBehavior(this);

	//set up the smoother
	pHeadingSmoother_ = new Smoother<Vector2D>(Prm.NumSamplesForSmoothing, Vector2D(0.0, 0.0));
}


//---------------------------- dtor -------------------------------------
//-----------------------------------------------------------------------
Vehicle::~Vehicle()
{
	delete pSteering_;
	delete pHeadingSmoother_;
}


//------------------------------ Update ----------------------------------
//
//  Updates the vehicle's position from a series of steering behaviors
//------------------------------------------------------------------------
void Vehicle::Update(double time_elapsed)
{
	//calculate the combined force from each steering behavior in the 
	//vehicle's list
	Vector2D steeringForce = pSteering_->Calculate();

	//Acceleration = Force/Mass
	Vector2D acceleration = steeringForce / dMass_;

	//update velocity
	velocity_ += acceleration * time_elapsed;

	//make sure vehicle does not exceed maximum velocity
	velocity_.Truncate(dMaxSpeed_);

	//update the position
	position_ += velocity_ * time_elapsed;

	//update the heading if the vehicle has a non zero velocity
	if (velocity_.LengthSq() > 0.000001)
	{
		headDirection_ = Vec2DNormalize(velocity_);

		sideDirection_ = headDirection_.Perp();
	}

	//EnforceNonPenetrationConstraint(this, World()->Agents());

	//treat the screen as a toroid
	WrapAround(position_, pOwnedWorld_->cxClient(), pOwnedWorld_->cyClient());

}