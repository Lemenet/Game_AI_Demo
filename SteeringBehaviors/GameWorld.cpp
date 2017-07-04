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
//#include "ParammLoader.h"
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
	pPath_(NULL),
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
	}
}
