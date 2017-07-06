#pragma once

//------------------------------------------------------------------------
//
//  Name:   GameWorld.h
//
//  Desc:   All the environment data and methods for the Steering
//          Behavior projects. This class is the root of the project's
//          update and render calls (excluding main of course)
//
//  Author: HuangMengdi 2017
//
//------------------------------------------------------------------------

#include <windows.h>
#include <vector>

#include "2D/Vector2D.h"
#include "Time/PrecisionTimer.h"
#include "misc/CellSpacePartition.h"
#include "BaseGameEntity.h"
//#include "EntityFunctionTemplates.h"
#include "Vehicle.h"


class Obstacle;
class Wall2D;
class Path;

typedef std::vector<BaseGameEntity*>::iterator Obit;

class GameWorld
{
private:

	//A container of all the moving entitis
	std::vector<Vehicle*> vAllVehicles_;

	//Any obstacles
	std::vector<BaseGameEntity*> vObstacles_;

	//Container containing any walls in the environment
	std::vector<Wall2D> vWalls_;

	CellSpacePartition<Vehicle*>* pCellSpace_;

	//Any path we may create for the vehicles to follow
	Path* pPath_;

	//Set true to pause the motion
	bool bPaused_;

	//Local copy of client window dimensions
	int nClientX_;
	int nClientY_;

	//The position of the crosshair;
	Vector2D crosshair_;

	//Keep track of the average FPS;
	double dAverageFrameTime_;

	//Flags to turn aids and obstacles etc on/off
	bool bShowWalls_;
	bool bShowObstacles_;
	bool bShowPath_;
	bool bShowDetectionBox_;
	bool bShowWanderCircle_;
	bool bShowFeelers_;
	bool bShowSteeringForce_;
	bool bShowFPS_;
	bool bShowRenderNeighbors_;
	bool bViewKeys_;
	bool bShowCellSpaceInfo_;

	void CreateObstacles();

	void CreateWalls();

public:

	GameWorld(int cx, int cy);

	~GameWorld();

	void Update(double timeElapsed);

	void Render();



	void  NonPenetrationContraint(Vehicle* v) { EnforceNonPenetrationConstraint(v, vAllVehicles_); }

	void  TagVehiclesWithinViewRange(BaseGameEntity* pVehicle, double range)
	{
		TagNeighbors(pVehicle, vAllVehicles_, range);
	}

	void  TagObstaclesWithinViewRange(BaseGameEntity* pVehicle, double range)
	{
		TagNeighbors(pVehicle, Obstacles(), range);
	}

	const std::vector<Wall2D>&          Walls() { return vWalls_; }
	CellSpacePartition<Vehicle*>*       CellSpace() { return pCellSpace_; }
	const std::vector<BaseGameEntity*>& Obstacles()const { return vObstacles_; }
	const std::vector<Vehicle*>&        Agents() { return vAllVehicles_; }



	//handle WM_COMMAND messages
	void HandleKeyPresses(WPARAM wParam);
	void HandleMenuItems(WPARAM wParam, HWND hwnd);

	void TogglePause() { bPaused_ = !bPaused_; }
	bool Paused()const { return bPaused_; }

	Vector2D Crosshair()const { return crosshair_; }
	void SetCrosshair(POINTS p);
	void SetCrosshair(Vector2D v) { crosshair_ = v; }

	int cxClient()const { return nClientX_; }
	int cyClient()const { return nClientY_; }

	bool RenderWalls()const { return bShowWalls_; }
	bool RenderObstacles()const { return bShowObstacles_; }
	bool RenderPath()const { return bShowPath_; }
	bool RenderDetectionBox()const { return bShowDetectionBox_; }
	bool RenderWanderCircle()const { return bShowWanderCircle_; }
	bool RenderFeelers()const { return bShowFeelers_; }
	bool RenderSteeringForce()const { return bShowSteeringForce_; }

	bool RenderFPS()const { return bShowFPS_; }
	void ToggleShowFPS() { bShowFPS_ = !bShowFPS_; }

	void ToggleRenderNeighbors() { bShowRenderNeighbors_ = !bShowRenderNeighbors_; }
	bool RenderNeighbors()const { return bShowRenderNeighbors_; }

	void ToggleViewKeys() { bViewKeys_ = !bViewKeys_; }
	bool ViewKeys()const { return bViewKeys_; }
};