#include <list>

#include "GameWorld.h"
#include "Vehicle.h"
#include "Constants.h"
//#include "Obstacle.h"
#include "2D/geometry.h"
#include "2D/Wall2D.h"
#include "2D/Transformations.h"
#include "SteeringBehavior.h"
#include "Time/PrecisionTimer.h"
#include "misc/Smoother.h"
#include "ParamLoader.h"
#include "misc/WindowUtils.h"
#include "misc/Stream_Utility_Functions.h"

#include "resource.h"

using std::list;

GameWorld::GameWorld(int cx, int cy) :
	nClientX_(cx),
	nClientY_(cy),
	bPaused_(false),
	crosshair_(Vector2D(cxClient() / 2.0, cxClient() / 2.0)),
	bShowWalls_(false),
	bShowObstacles_(false),
	bShowPath_(false),
	bShowWanderCircle_(false),
	bShowSteeringForce_(false),
	bShowFeelers_(false),
	bShowDetectionBox_(false),
	bShowFPS_(true),
	dAverageFrameTime_(0),
	pPath_(nullptr),
	bShowRenderNeighbors_(false),
	bViewKeys_(false),
	bShowCellSpaceInfo_(false)
{
	//Setup the spatial subdivision class
	pCellSpace_ = new CellSpacePartition<Vehicle*>((double)cx, (double)cy, Prm.NumCellsX, Prm.NumCellsY, Prm.NumAgents);

	double border = 30;
	pPath_ = new Path(5, border, border, cx - border, cy - border, true);

	//Setup the agents
	for (int a = 0; a < Prm.NumAgents; ++a)
	{
		//determine a random starting position
		Vector2D SpawnPos = Vector2D(cx / 2.0 + RandomClamped()*cx / 2.0, 
									cy / 2.0 + RandomClamped()*cy / 2.0);

		Vehicle* pVehicle = new Vehicle(this,
			SpawnPos,					//Initial position
			RandFloat()*TwoPi,			//Start rotation
			Vector2D(0, 0),				//Velocity
			Prm.VehicleMass,			//Mass
			Prm.MaxSteeringForce,		//Max force
			Prm.MaxSpeed,				//Max velocity
			Prm.MaxTurnRatePerSecond,	//Max turn rate
			Prm.VehicleScale			//Scale
		);

		pVehicle->Steering()->FlockingOn();
		vAllVehicles_.push_back(pVehicle);

		//Add if to the cell subdivision
		pCellSpace_->AddEntity(pVehicle);
	}


#define SHOAL
#ifdef SHOAL
// 	vAllVehicles_[Prm.NumAgents - 1]->Steering()->FlockingOff();
// 	vAllVehicles_[Prm.NumAgents - 1]->SetScale(Vector2D(10, 10));
// 	vAllVehicles_[Prm.NumAgents - 1]->Steering()->WanderOn();
// 	vAllVehicles_[Prm.NumAgents - 1]->SetMaxSpeed(70);
// 
// 	for (int i = 0; i < Prm.NumAgents - 1; ++i)
// 	{
// 		vAllVehicles_[i]->Steering()->EvadeOn(vAllVehicles_[Prm.NumAgents - 1]);
// 	}

#endif
	
	//create any obstacles or walls
	//CreateObstacles();
	//CreateWalls();

}

//-------------------------------- dtor ----------------------------------
//------------------------------------------------------------------------
GameWorld::~GameWorld()
{
	for (unsigned int a = 0; a < vAllVehicles_.size(); ++a)
	{
		delete vAllVehicles_[a];
	}

	for (unsigned int ob = 0; ob < vObstacles_.size(); ++ob)
	{
		delete vObstacles_[ob];
	}

	delete pCellSpace_;

	delete pPath_;
}

//----------------------------- Update -----------------------------------
//------------------------------------------------------------------------
void GameWorld::Update(double time_elapsed)
{
	if (bPaused_)
	{
		return;
	}

	//Create a smoother to smooth the framerate
	const int SampleRate = 10;
	static Smoother<double> FrameRateSmoother(SampleRate, 0.0);

	dAverageFrameTime_ = FrameRateSmoother.Update(time_elapsed);

	//Update the vehicles
	for (unsigned int a = 0; a < vAllVehicles_.size(); ++a)
	{
		vAllVehicles_[a]->Update(time_elapsed);
	}
}