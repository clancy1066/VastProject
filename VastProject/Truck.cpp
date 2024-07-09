
#include <iostream>
#include "SimDefinitions.h"
#include "Truck.h"

int  Truck::_sTruckIDCount = 0;


Truck::Truck()
{
	mID = _sTruckIDCount++;
}

Truck::~Truck()
{
	std::cout << "Destructor of Truck" << std::endl;
}

// *****************************************
// Returns true if it it is done processing
// *****************************************
bool Truck::Execute(float dt)
{
	bool retVal = false;;

	switch (mState)
	{
	case TRUCK_STATE::IDLE:			retVal = UpdateIDLE(dt);		break;
	case TRUCK_STATE::PRESTART:		retVal = UpdatePRESTART(dt);	break;
	case TRUCK_STATE::LOADING:		retVal = UpdateLOADING(dt);		break;
	case TRUCK_STATE::TO_MINE:		retVal = UpdateTO_MINE(dt);		break;
	case TRUCK_STATE::FROM_MINE:	retVal = UpdateFROM_MINE(dt);	break;
	case TRUCK_STATE::UNLOADING:	retVal = UpdateUNLOADING(dt);	break;
	}


	return retVal;
}

void Truck::StartMission(float waitTime,float extractTime)
{
	ChangeState(PRESTART);
	mWaitTimeTilNextState	= waitTime;
	mExtractTime			= extractTime;
}

void Truck::ReturnToStation()
{
	ChangeState(TRUCK_STATE::FROM_MINE);
}

void Truck::ChangeState(TRUCK_STATE newState)
{
	mLastState		= mState;
	mState			= newState;
	mStateChanged	= true;

	std::cout << "Truck " << mID << " change state to " << StateEnumString(mState) << std::endl;
}

bool Truck::StateChanged()
{
	bool retVal = mStateChanged;

	mStateChanged = false;

	return retVal;
}


//  ******************************************
//	State Machine
//  ******************************************
bool Truck::UpdateIDLE(float dt)
{
	// return true; This will force them back to the idle list
	return true;
}

bool Truck::UpdatePRESTART(float dt)
{

	mWaitTimeTilNextState -= dt;

	if (mWaitTimeTilNextState <= 0.0f)
		ChangeState(TRUCK_STATE::TO_MINE);

	return false;
}

bool Truck::UpdateTO_MINE(float dt)
{
	if (StateChanged())
	{
		mWaitTimeTilNextState = SIM_TRAVEL_TIME_IN_SECONDS_F-dt;
		return false;
	}

	mWaitTimeTilNextState -= dt;

	if (mWaitTimeTilNextState <= 0.0f)
		ChangeState(TRUCK_STATE::LOADING);

	return false;
}

bool Truck::UpdateLOADING(float dt)
{
	if (StateChanged())
	{
		mWaitTimeTilNextState = SIM_MAX_EXTRACT_TIME_IN_SECONDS_F - dt;
		return false;
	}

	mWaitTimeTilNextState -= dt;

	if (mWaitTimeTilNextState <= 0.0f)
		ChangeState(TRUCK_STATE::FROM_MINE);

	return false;
}

bool Truck::UpdateFROM_MINE(float dt)
{
	if (StateChanged())
	{
		mWaitTimeTilNextState = SIM_TRAVEL_TIME_IN_SECONDS_F -dt;
		return false;
	}

	mWaitTimeTilNextState -= dt;

	if (mWaitTimeTilNextState <= 0.0f)
		ChangeState(TRUCK_STATE::UNLOADING);

	return false;
}

bool Truck::UpdateUNLOADING(float dt)
{
	if (StateChanged())
	{
		mWaitTimeTilNextState = mLoadingTime;
		return false;
	}

	mWaitTimeTilNextState -= dt;

	if (mWaitTimeTilNextState <= 0.0f)
	{
		std::cout << "TRUCK RETURNS WITH " << mCollectedOre << " ore. ********************************" << std::endl;
		ChangeState(TRUCK_STATE::IDLE);
	}
	return false;
}



