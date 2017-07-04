#pragma once


//------------------------------------------------------------------------
//
//  Name:   Vehicle.h
//
//  Desc:   Definition of a simple vehicle that uses steering behaviors
//
//  Author: HuangMengdi 2017
//
//------------------------------------------------------------------------

#include <vector>
#include <list>
#include <string>

#include "MovingEntity.h"
#include "2D/Vector2D.h"
#include "misc/Smoother.h"

class GameWorld;
class SteeringBehavior;

class Vehicle : public MovingEntity
{

private:

	//a pointer to the world data. So a vehicle can access any obstacle,
	//path, wall or agent data
	GameWorld* pOwnedWorld_;

	//the steering behavior class
	SteeringBehavior* pSteering_;

	//some steering behaviors give jerky looking movement. The
	//following members are used to smooth the vehicle's heading
	Smoother<Vector2D>* pHeadingSmoother_;

	//this vector represents the average of the vehicle's heading
	//vector smoothed over the last few frames
	Vector2D smoothedHeading_;

	//If true, the smooth is active
	bool bSmoothingOn_;

	//keeps a track of the most recent update time. (some of the
	//steering behaviors make use of this - see Wander)
	double dTimeElapsed_;

	//buffer for the vehicle shape
	std::vector<Vector2D> vVehicleShapeBuffer_;

	//fills the buffer with vertex data
	void InitializeBuffer();

	//disallow the copying of Vehicle types
	Vehicle(const Vehicle&);
	Vehicle& operator=(const Vehicle&);

public:

	Vehicle(
		GameWorld* world,
		Vector2D position,
		double rotation,
		Vector2D velocity,
		double mass,
		double max_force,
		double max_speed,
		double max_turn_rate,
		double scale
	);

	~Vehicle();

	//updates the vehicle's position and orientation
	void Update(double time_elapsed);

	void Render();


	//-------------------------------------------accessor methods
	
	//pSteering_
	SteeringBehavior* const Steering()const { return pSteering_; }
	GameWorld* const World()const { return pOwnedWorld_; }

	//smoothedHeading_
	Vector2D SmoothedHeading() const { return smoothedHeading_; }

	//bSmoothingOn_
	bool IsSmoothingOn() const { return bSmoothingOn_; }
	void SmoothOn() { bSmoothingOn_ = true; }
	void SmoothOff() { bSmoothingOn_ = false; }
	void ToggleSmoothing() { bSmoothingOn_ = !bSmoothingOn_; }

	double TimeElapsed() const { return dTimeElapsed_; }
};