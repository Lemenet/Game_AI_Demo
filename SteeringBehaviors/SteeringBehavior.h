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
//#include "Path.h"
//#include "ParamLoader.h"

class Vehicle;
class CController;
class Wall2D;
class BaseGameEntity;

//---------------------- Constants ----------------------

//The radius fot the constraining circle for the wander behavior
double WanderRadius = 1.2;

//-------------------------------------------------------


class SteeringBehavior
{

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






	/* .......................................................

	BEGIN BEHAVIOR DECLARATIONS

	.......................................................*/


	//This behavior moves the agent towards a target position
	Vector2D Seek(Vector2D TargetPos);

	//This behavior returns a vector that moves the agent away
	//from a target position
	Vector2D Flee(Vector2D TargetPos);

};