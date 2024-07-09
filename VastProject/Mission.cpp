#include <iostream>
#include <sstream>

#include "Mission.h"
#include "Station.h"

// ID management
int  Mission::_sMissionIDCount	= 0;


const char* StateEnumString(MISSION_STATE state)
{

	switch (state)
	{
	case MISSION_STATE::IDLE:		return "IDLE";
	case MISSION_STATE::PRESTART:	return "PRESTART";
	case MISSION_STATE::TO_MINE:	return "TO_MINE";
	case MISSION_STATE::LOADING:	return "LOADING";
	case MISSION_STATE::FROM_MINE:	return "FROM_MINE";
	case MISSION_STATE::UNLOADING:	return "UNLOADING";
	}
	return "<none>";
}

// *****************************************************
//	State machine
// *****************************************************
void Mission::ChangeState(MISSION_STATE newState)
{
	mLastState = mState;
	mState = newState;
	mStateChanged = true;

	std::string timeString = TimeString(mStartTime + mElapsedTime);

	std::cout << "Mission " << mID << " change state to " << StateEnumString(mState) << " Time: " << timeString << std::endl;
	std::cout << "    *** Truck " << mTruckID ;
	std::cout << " from station " << mStation->GetID();
	std::cout << " to Mine " << mMineID << std::endl;

}

bool Mission::StateChanged()
{
	bool retVal = mStateChanged;

	mStateChanged = false;

	return retVal;
}

std::string Mission::TimeString(float timeInSeconds)
{
	std::string retVal = "";

	int hours = (int)(timeInSeconds / 3600.0f);
	int mins = (int)(timeInSeconds / 60.0f) % 60;
	int secs = (int)(timeInSeconds) % 60;

	// Convert integers to strings using stringstream
	std::stringstream ssHours, ssMins, ssSecs;
	ssHours << hours;
	ssMins << mins;
	ssSecs << secs;

	retVal += (ssHours.str());
	retVal += (":" + ssMins.str());
	retVal += ("::" + ssSecs.str());

	return retVal;
}

bool Mission::Begin(int truckID, int mineID, Station* station, float startTime)
{
	if ((truckID < 0) || (mineID < 0) || (station == NULL))
		return false;;

	mTruckID = truckID;
	mMineID = mineID;
	mStation = station;
	mStartTime = startTime;

	mMineWasFreed	= false;
	mCanFreeMine = false;

	mCollectedOre = 0;

	ChangeState(MISSION_STATE::PRESTART);

	return true;
}

// *****************************************
// Returns true if it it is done processing
// *****************************************
bool Mission::Execute(float dt)
{
	bool retVal = false;;

	switch (mState)
	{
	case MISSION_STATE::IDLE:			retVal = UpdateIDLE(dt);		break;
	case MISSION_STATE::PRESTART:		retVal = UpdatePRESTART(dt);	break;
	case MISSION_STATE::TO_MINE:		retVal = UpdateTO_MINE(dt);		break;
	case MISSION_STATE::LOADING:		retVal = UpdateLOADING(dt);		break;
	case MISSION_STATE::FROM_MINE:		retVal = UpdateFROM_MINE(dt);	break;
	case MISSION_STATE::UNLOADING:		retVal = UpdateUNLOADING(dt);	break;
	}

	mElapsedTime += dt;

	return retVal;
}


//  ******************************************
//	State Machine Helpers
//  ******************************************
bool Mission::UpdateIDLE(float dt)
{
	// return true; This will force them back to the idle list
	return false;
}

bool Mission::UpdatePRESTART(float dt)
{
	if (StateChanged())
	{
		mWaitTimeTilNextState =  dt;
		return false;
	}

	mWaitTimeTilNextState -= dt;

	if (mWaitTimeTilNextState <= 0.0f)
		ChangeState(MISSION_STATE::TO_MINE);

	return false;
}

bool Mission::UpdateTO_MINE(float dt)
{
	if (StateChanged())
	{
		mWaitTimeTilNextState = SIM_TRAVEL_TIME_IN_SECONDS_F - dt;
		return false;
	}

	mWaitTimeTilNextState -= dt;

	if (mWaitTimeTilNextState <= 0.0f)
		ChangeState(MISSION_STATE::LOADING);

	return false;
}

bool Mission::UpdateLOADING(float dt)
{
	if (StateChanged())
	{
		mWaitTimeTilNextState = dt; SIM_MAX_EXTRACT_TIME_IN_SECONDS_F;
		return false;
	}

	mWaitTimeTilNextState += dt;
		

	// Free the mine when there is only SIM_TRAVEL_TIME_IN_SECONDS_F left.
	// This will cause the next truck to issue to this mine on the next frame
	if (mWaitTimeTilNextState >= SIM_MAX_EXTRACT_TIME_IN_SECONDS_F - SIM_TRAVEL_TIME_IN_SECONDS_F)
	{
		mCanFreeMine = true;
	}

	if (mWaitTimeTilNextState >= SIM_MAX_EXTRACT_TIME_IN_SECONDS_F)
	{
		ChangeState(MISSION_STATE::FROM_MINE);
	}
	return false;
}

bool Mission::UpdateFROM_MINE(float dt)
{
	if (StateChanged())
	{
		mWaitTimeTilNextState = SIM_TRAVEL_TIME_IN_SECONDS_F - dt;
		return false;
	}

	mWaitTimeTilNextState -= dt;

	if (mWaitTimeTilNextState <= 0.0f)
		ChangeState(MISSION_STATE::UNLOADING);

	return false;
}

bool Mission::UpdateUNLOADING(float dt)
{
	if (StateChanged())
	{
		mWaitTimeTilNextState = SIM_WAIT_UNLOAD_IN_SECONDS_F;
		return false;
	}

	mWaitTimeTilNextState -= dt;

	if (mWaitTimeTilNextState <= 0.0f)
	{
		mCollectedOre = 5;
		std::cout << "MISSION ENDS WITH " << mCollectedOre << " ore. ********************************" << std::endl;
		ChangeState(MISSION_STATE::IDLE);

		return true;
	}
	return false;
}