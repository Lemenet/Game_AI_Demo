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
	dWeightCohesion_(Prm.CohesionWeight),
	dWeightAlignment_(Prm.AlignmentWeight),
	dWeightSeparation_(Prm.SeparationWeight),
	dWeightObstacleAvoidance_(Prm.ObstacleAvoidanceWeight),
	dWeightWander_(Prm.WanderWeight),
	dWeightWallAvoidance_(Prm.WallAvoidanceWeight),
	m_dViewDistance(Prm.ViewDistance),
	m_dWallDetectionFeelerLength(Prm.WallDetectionFeelerLength),
	m_Feelers(3),
	m_Deceleration(normal),
	pTargetAgent1_(NULL),
	pTargetAgent2_(NULL),
	m_dWanderDistance(WanderDist),
	m_dWanderJitter(WanderJitterPerSec),
	m_dWanderRadius(WanderRad),
	m_dWaypointSeekDistSq(WaypointSeekDist*WaypointSeekDist),
	dWeightSeek_(Prm.SeekWeight),
	dWeightFlee_(Prm.FleeWeight),
	dWeightArrive_(Prm.ArriveWeight),
	dWeightPursuit_(Prm.PursuitWeight),
	dWeightOffsetPursuit_(Prm.OffsetPursuitWeight),
	dWeightInterpose_(Prm.InterposeWeight),
	dWeightHide_(Prm.HideWeight),
	dWeightEvade_(Prm.EvadeWeight),
	dWeightFollowPath_(Prm.FollowPathWeight),
	m_bCellSpaceOn(false),
	m_SummingMethod(prioritized)


{
	//stuff for the wander behavior
	double theta = RandFloat() * TwoPi;

	//create a vector to a target position on the wander circle
	m_vWanderTarget = Vector2D(m_dWanderRadius * cos(theta),
		m_dWanderRadius * sin(theta));

	//create a Path
	pPath_ = new Path();
	pPath_->LoopOn();

}

//---------------------------------dtor ----------------------------------
SteeringBehavior::~SteeringBehavior() { delete pPath_; }


/////////////////////////////////////////////////////////////////////////////// CALCULATE METHODS 


//----------------------- Calculate --------------------------------------
//
//  calculates the accumulated steering force according to the method set
//  in m_SummingMethod
//------------------------------------------------------------------------
Vector2D SteeringBehavior::Calculate()
{
	//reset the steering force
	vSteeringForce_.Zero();

	//use space partitioning to calculate the neighbours of this vehicle
	//if switched on. If not, use the standard tagging system
	if (!isSpacePartitioningOn())
	{
		//tag neighbors if any of the following 3 group behaviors are switched on
		if (On(separation) || On(allignment) || On(cohesion))
		{
			pOwnerVehicle_->World()->TagVehiclesWithinViewRange(pVehicle_, m_dViewDistance);
		}
	}
	else
	{
		//calculate neighbours in cell-space if any of the following 3 group
		//behaviors are switched on
		if (On(separation) || On(allignment) || On(cohesion))
		{
			pOwnerVehicle_->World()->CellSpace()->CalculateNeighbors(pOwnerVehicle_->Pos(), m_dViewDistance);
		}
	}

	switch (m_SummingMethod)
	{
	case weighted_average:

		vSteeringForce_ = CalculateWeightedSum(); break;

	case prioritized:

		vSteeringForce_ = CalculatePrioritized(); break;

	case dithered:

		vSteeringForce_ = CalculateDithered(); break;

	default:vSteeringForce_ = Vector2D(0, 0);

	}//end switch

	return vSteeringForce_;
}


//------------------------- ForwardComponent -----------------------------
//
//  returns the forward oomponent of the steering force
//------------------------------------------------------------------------
double SteeringBehavior::ForwardComponent()
{
	return pVehicle_->Heading().Dot(vSteeringForce_);
}

//--------------------------- SideComponent ------------------------------
//  returns the side component of the steering force
//------------------------------------------------------------------------
double SteeringBehavior::SideComponent()
{
	return pVehicle_->Side().Dot(vSteeringForce_);
}


//--------------------- AccumulateForce ----------------------------------
//
//  This function calculates how much of its max steering force the 
//  vehicle has left to apply and then applies that amount of the
//  force to add.
//------------------------------------------------------------------------
bool SteeringBehavior::AccumulateForce(Vector2D &RunningTot,
	Vector2D ForceToAdd)
{

	//calculate how much steering force the vehicle has used so far
	double MagnitudeSoFar = RunningTot.Length();

	//calculate how much steering force remains to be used by this vehicle
	double MagnitudeRemaining = pVehicle_->MaxForce() - MagnitudeSoFar;

	//return false if there is no more force left to use
	if (MagnitudeRemaining <= 0.0) return false;

	//calculate the magnitude of the force we want to add
	double MagnitudeToAdd = ForceToAdd.Length();

	//if the magnitude of the sum of ForceToAdd and the running total
	//does not exceed the maximum force available to this vehicle, just
	//add together. Otherwise add as much of the ForceToAdd vector is
	//possible without going over the max.
	if (MagnitudeToAdd < MagnitudeRemaining)
	{
		RunningTot += ForceToAdd;
	}

	else
	{
		//add it to the steering force
		RunningTot += (Vec2DNormalize(ForceToAdd) * MagnitudeRemaining);
	}

	return true;
}



//---------------------- CalculatePrioritized ----------------------------
//
//  this method calls each active steering behavior in order of priority
//  and acumulates their forces until the max steering force magnitude
//  is reached, at which time the function returns the steering force 
//  accumulated to that  point
//------------------------------------------------------------------------
Vector2D SteeringBehavior::CalculatePrioritized()
{
	Vector2D force;

	if (On(wall_avoidance))
	{
		force = WallAvoidance(pVehicle_->World()->Walls()) *
			dWeightWallAvoidance_;

		if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
	}

	if (On(obstacle_avoidance))
	{
		force = ObstacleAvoidance(pVehicle_->World()->Obstacles()) *
			dWeightObstacleAvoidance_;

		if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
	}

	if (On(evade))
	{
		assert(pTargetAgent1_ && "Evade target not assigned");

		force = Evade(pTargetAgent1_) * dWeightEvade_;

		if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
	}


	if (On(flee))
	{
		force = Flee(pVehicle_->World()->Crosshair()) * dWeightFlee_;

		if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
	}



	//these next three can be combined for flocking behavior (wander is
	//also a good behavior to add into this mix)
	if (!isSpacePartitioningOn())
	{
		if (On(separation))
		{
			force = Separation(pVehicle_->World()->Agents()) * dWeightSeparation_;

			if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
		}

		if (On(allignment))
		{
			force = Alignment(pVehicle_->World()->Agents()) * dWeightAlignment_;

			if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
		}

		if (On(cohesion))
		{
			force = Cohesion(pVehicle_->World()->Agents()) * dWeightCohesion_;

			if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
		}
	}

	else
	{

		if (On(separation))
		{
			force = SeparationPlus(pVehicle_->World()->Agents()) * dWeightSeparation_;

			if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
		}

		if (On(allignment))
		{
			force = AlignmentPlus(pVehicle_->World()->Agents()) * dWeightAlignment_;

			if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
		}

		if (On(cohesion))
		{
			force = CohesionPlus(pVehicle_->World()->Agents()) * dWeightCohesion_;

			if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
		}
	}

	if (On(seek))
	{
		force = Seek(pVehicle_->World()->Crosshair()) * dWeightSeek_;

		if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
	}


	if (On(arrive))
	{
		force = Arrive(pVehicle_->World()->Crosshair(), m_Deceleration) * dWeightArrive_;

		if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
	}

	if (On(wander))
	{
		force = Wander() * dWeightWander_;

		if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
	}

	if (On(pursuit))
	{
		assert(pTargetAgent1_ && "pursuit target not assigned");

		force = Pursuit(pTargetAgent1_) * dWeightPursuit_;

		if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
	}

	if (On(offset_pursuit))
	{
		assert(pTargetAgent1_ && "pursuit target not assigned");
		assert(!m_vOffset.isZero() && "No offset assigned");

		force = OffsetPursuit(pTargetAgent1_, m_vOffset);

		if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
	}

	if (On(interpose))
	{
		assert(pTargetAgent1_ && pTargetAgent2_ && "Interpose agents not assigned");

		force = Interpose(pTargetAgent1_, pTargetAgent2_) * dWeightInterpose_;

		if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
	}

	if (On(hide))
	{
		assert(pTargetAgent1_ && "Hide target not assigned");

		force = Hide(pTargetAgent1_, pVehicle_->World()->Obstacles()) * dWeightHide_;

		if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
	}


	if (On(follow_path))
	{
		force = FollowPath() * dWeightFollowPath_;

		if (!AccumulateForce(vSteeringForce_, force)) return vSteeringForce_;
	}

	return vSteeringForce_;
}



//---------------------- CalculateWeightedSum ----------------------------
//
//  this simply sums up all the active behaviors X their weights and 
//  truncates the result to the max available steering force before 
//  returning
//------------------------------------------------------------------------
Vector2D SteeringBehavior::CalculateWeightedSum()
{
	if (On(wall_avoidance))
	{
		vSteeringForce_ += WallAvoidance(pVehicle_->World()->Walls()) *
			dWeightWallAvoidance_;
	}

	if (On(obstacle_avoidance))
	{
		vSteeringForce_ += ObstacleAvoidance(pVehicle_->World()->Obstacles()) *
			dWeightObstacleAvoidance_;
	}

	if (On(evade))
	{
		assert(pTargetAgent1_ && "Evade target not assigned");

		vSteeringForce_ += Evade(pTargetAgent1_) * dWeightEvade_;
	}


	//these next three can be combined for flocking behavior (wander is
	//also a good behavior to add into this mix)
	if (!isSpacePartitioningOn())
	{
		if (On(separation))
		{
			vSteeringForce_ += Separation(pVehicle_->World()->Agents()) * dWeightSeparation_;
		}

		if (On(allignment))
		{
			vSteeringForce_ += Alignment(pVehicle_->World()->Agents()) * dWeightAlignment_;
		}

		if (On(cohesion))
		{
			vSteeringForce_ += Cohesion(pVehicle_->World()->Agents()) * dWeightCohesion_;
		}
	}
	else
	{
		if (On(separation))
		{
			vSteeringForce_ += SeparationPlus(pVehicle_->World()->Agents()) * dWeightSeparation_;
		}

		if (On(allignment))
		{
			vSteeringForce_ += AlignmentPlus(pVehicle_->World()->Agents()) * dWeightAlignment_;
		}

		if (On(cohesion))
		{
			vSteeringForce_ += CohesionPlus(pVehicle_->World()->Agents()) * dWeightCohesion_;
		}
	}


	if (On(wander))
	{
		vSteeringForce_ += Wander() * dWeightWander_;
	}

	if (On(seek))
	{
		vSteeringForce_ += Seek(pVehicle_->World()->Crosshair()) * dWeightSeek_;
	}

	if (On(flee))
	{
		vSteeringForce_ += Flee(pVehicle_->World()->Crosshair()) * dWeightFlee_;
	}

	if (On(arrive))
	{
		vSteeringForce_ += Arrive(pVehicle_->World()->Crosshair(), m_Deceleration) * dWeightArrive_;
	}

	if (On(pursuit))
	{
		assert(pTargetAgent1_ && "pursuit target not assigned");

		vSteeringForce_ += Pursuit(pTargetAgent1_) * dWeightPursuit_;
	}

	if (On(offset_pursuit))
	{
		assert(pTargetAgent1_ && "pursuit target not assigned");
		assert(!m_vOffset.isZero() && "No offset assigned");

		vSteeringForce_ += OffsetPursuit(pTargetAgent1_, m_vOffset) * dWeightOffsetPursuit_;
	}

	if (On(interpose))
	{
		assert(pTargetAgent1_ && pTargetAgent2_ && "Interpose agents not assigned");

		vSteeringForce_ += Interpose(pTargetAgent1_, pTargetAgent2_) * dWeightInterpose_;
	}

	if (On(hide))
	{
		assert(pTargetAgent1_ && "Hide target not assigned");

		vSteeringForce_ += Hide(pTargetAgent1_, pVehicle_->World()->Obstacles()) * dWeightHide_;
	}

	if (On(follow_path))
	{
		vSteeringForce_ += FollowPath() * dWeightFollowPath_;
	}

	vSteeringForce_.Truncate(pVehicle_->MaxForce());

	return vSteeringForce_;
}


//---------------------- CalculateDithered ----------------------------
//
//  this method sums up the active behaviors by assigning a probabilty
//  of being calculated to each behavior. It then tests the first priority
//  to see if it should be calcukated this simulation-step. If so, it
//  calculates the steering force resulting from this behavior. If it is
//  more than zero it returns the force. If zero, or if the behavior is
//  skipped it continues onto the next priority, and so on.
//
//  NOTE: Not all of the behaviors have been implemented in this method,
//        just a few, so you get the general idea
//------------------------------------------------------------------------
Vector2D SteeringBehavior::CalculateDithered()
{
	//reset the steering force
	vSteeringForce_.Zero();

	if (On(wall_avoidance) && RandFloat() < Prm.prWallAvoidance)
	{
		vSteeringForce_ = WallAvoidance(pVehicle_->World()->Walls()) *
			dWeightWallAvoidance_ / Prm.prWallAvoidance;

		if (!vSteeringForce_.isZero())
		{
			vSteeringForce_.Truncate(pVehicle_->MaxForce());

			return vSteeringForce_;
		}
	}

	if (On(obstacle_avoidance) && RandFloat() < Prm.prObstacleAvoidance)
	{
		vSteeringForce_ += ObstacleAvoidance(pVehicle_->World()->Obstacles()) *
			dWeightObstacleAvoidance_ / Prm.prObstacleAvoidance;

		if (!vSteeringForce_.isZero())
		{
			vSteeringForce_.Truncate(pVehicle_->MaxForce());

			return vSteeringForce_;
		}
	}

	if (!isSpacePartitioningOn())
	{
		if (On(separation) && RandFloat() < Prm.prSeparation)
		{
			vSteeringForce_ += Separation(pVehicle_->World()->Agents()) *
				dWeightSeparation_ / Prm.prSeparation;

			if (!vSteeringForce_.isZero())
			{
				vSteeringForce_.Truncate(pVehicle_->MaxForce());

				return vSteeringForce_;
			}
		}
	}

	else
	{
		if (On(separation) && RandFloat() < Prm.prSeparation)
		{
			vSteeringForce_ += SeparationPlus(pVehicle_->World()->Agents()) *
				dWeightSeparation_ / Prm.prSeparation;

			if (!vSteeringForce_.isZero())
			{
				vSteeringForce_.Truncate(pVehicle_->MaxForce());

				return vSteeringForce_;
			}
		}
	}


	if (On(flee) && RandFloat() < Prm.prFlee)
	{
		vSteeringForce_ += Flee(pVehicle_->World()->Crosshair()) * dWeightFlee_ / Prm.prFlee;

		if (!vSteeringForce_.isZero())
		{
			vSteeringForce_.Truncate(pVehicle_->MaxForce());

			return vSteeringForce_;
		}
	}

	if (On(evade) && RandFloat() < Prm.prEvade)
	{
		assert(pTargetAgent1_ && "Evade target not assigned");

		vSteeringForce_ += Evade(pTargetAgent1_) * dWeightEvade_ / Prm.prEvade;

		if (!vSteeringForce_.isZero())
		{
			vSteeringForce_.Truncate(pVehicle_->MaxForce());

			return vSteeringForce_;
		}
	}


	if (!isSpacePartitioningOn())
	{
		if (On(allignment) && RandFloat() < Prm.prAlignment)
		{
			vSteeringForce_ += Alignment(pVehicle_->World()->Agents()) *
				dWeightAlignment_ / Prm.prAlignment;

			if (!vSteeringForce_.isZero())
			{
				vSteeringForce_.Truncate(pVehicle_->MaxForce());

				return vSteeringForce_;
			}
		}

		if (On(cohesion) && RandFloat() < Prm.prCohesion)
		{
			vSteeringForce_ += Cohesion(pOwnerVehicle_->World()->Agents()) *
				dWeightCohesion_ / Prm.prCohesion;

			if (!vSteeringForce_.isZero())
			{
				vSteeringForce_.Truncate(pOwnerVehicle_->MaxForce());

				return vSteeringForce_;
			}
		}
	}
	else
	{
		if (On(allignment) && RandFloat() < Prm.prAlignment)
		{
			vSteeringForce_ += AlignmentPlus(pVehicle_->World()->Agents()) *
				dWeightAlignment_ / Prm.prAlignment;

			if (!vSteeringForce_.isZero())
			{
				vSteeringForce_.Truncate(pVehicle_->MaxForce());

				return vSteeringForce_;
			}
		}

		if (On(cohesion) && RandFloat() < Prm.prCohesion)
		{
			vSteeringForce_ += CohesionPlus(pVehicle_->World()->Agents()) *
				dWeightCohesion_ / Prm.prCohesion;

			if (!vSteeringForce_.isZero())
			{
				vSteeringForce_.Truncate(pVehicle_->MaxForce());

				return vSteeringForce_;
			}
		}
	}

	if (On(wander) && RandFloat() < Prm.prWander)
	{
		vSteeringForce_ += Wander() * dWeightWander_ / Prm.prWander;

		if (!vSteeringForce_.isZero())
		{
			vSteeringForce_.Truncate(pVehicle_->MaxForce());

			return vSteeringForce_;
		}
	}

	if (On(seek) && RandFloat() < Prm.prSeek)
	{
		vSteeringForce_ += Seek(pVehicle_->World()->Crosshair()) * dWeightSeek_ / Prm.prSeek;

		if (!vSteeringForce_.isZero())
		{
			vSteeringForce_.Truncate(pVehicle_->MaxForce());

			return vSteeringForce_;
		}
	}

	if (On(arrive) && RandFloat() < Prm.prArrive)
	{
		vSteeringForce_ += Arrive(pVehicle_->World()->Crosshair(), m_Deceleration) *
			dWeightArrive_ / Prm.prArrive;

		if (!vSteeringForce_.isZero())
		{
			vSteeringForce_.Truncate(pVehicle_->MaxForce());

			return vSteeringForce_;
		}
	}

	return vSteeringForce_;
}


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
