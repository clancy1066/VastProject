#pragma once
#include "SimDefinitions.h"

// Forward decls
class Station;

enum MISSION_STATE
{
	IDLE = 0,
	PRESTART,
	TO_MINE,
	LOADING,
	FROM_MINE,
	UNLOADING

};

class Mission
{
public:

	Mission()
	{
		mStation	= nullptr;
		mID = _sMissionIDCount++;
	};

protected:

	~Mission();

public:

	bool		Begin(int truckID, int mineID,Station* station,float startTime);
	bool		Execute(float dt);

	float		GetElasedTime() { return mElapsedTime; }

	int			GetID() { return mID; }

	int			GetTruckID(){ return mTruckID;};
	int			GetMineID() { return mMineID; };
	Station*	GetStation(){ return mStation;};

	float		GetCollectedOre() { return mCollectedOre; };

	bool		CanFreeMine() 
	{
		if (mMineWasFreed)
			return false;

		return mCanFreeMine; 
	}

	void		FreeMine() { mMineWasFreed = true; }

protected:

	static int  _sMissionIDCount;
	int			mID = 0;

	// Principals
	int			mTruckID = -1;
	int			mMineID  = -1;
	Station*	mStation;
	
	
	// All ore
	float			mCollectedOre;

	float			mStartTime = 0;
	float			mElapsedTime = 0;
	bool			mCanFreeMine = false;
	bool			mMineWasFreed = false;
	

	// State machine
	void			ChangeState(MISSION_STATE newState);
	bool			StateChanged();
	std::string		TimeString(float timeInSeconds);
	
	MISSION_STATE	mState			= MISSION_STATE::IDLE;
	MISSION_STATE	mLastState		= MISSION_STATE::IDLE;
	bool			mStateChanged	= false;
	float			mWaitTimeTilNextState = 0;

//  ******************************************
//	State Machine Helpers
//  ******************************************
	bool UpdateIDLE			(float dt);
	bool UpdatePRESTART		(float dt);
	bool UpdateTO_MINE		(float dt);
	bool UpdateLOADING		(float dt);
	bool UpdateFROM_MINE	(float dt);
	bool UpdateUNLOADING	(float dt);



};