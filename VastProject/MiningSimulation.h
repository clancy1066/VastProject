#pragma once
#include <vector>
#include "SimDefinitions.h"
#include "Mission.h"


/*
	Global:
	total ore  int

	List of all trucks
	List of all mines
	List of all stations
*/

class MiningSimulation
{

public:

	int					mNumTrucks	= 2;
	int					mNumMines	= 2;
	int					mNumStations = 1;

	float				mSimulationTime = SIM_MAX_TIME_IN_SECONDS_F;

	// Public variable but still wrap it
	float				GetTotalSimulationTime() { return mSimulationTime; }

	float				GetCollectedOre() { return mCollectedOre; }
	
	Mission*			AllocMission();
	void				FreeMission(Mission*);

	void				EndMission(Mission*);

	// Fixed dt set externally
	void				SetDeltaTime(float dt) { mDeltaTime = dt; }

	void				QueryParameters();

protected:

	// ID assignment
	static int				_sTruckIDCount;
	static int				_sMineIDCount;

	// Progress and meomory tracking
	std::vector<int>		mAllTrucks;
	std::vector<int>		mAllMines;
	std::vector<Station*>   mAllStations;

	std::vector<Mission*>	mActiveMissions;
	std::vector<Mission*>	mFreeMissionPool;
	std::vector<Mission*>	mMissionsToRemove;

	float					mCollectedOre = 0;

	float					mDeltaTime = SIM_EVAL_GRANULAROTY_SECONDS_F;
	float					mElapsedTime = 0;


	bool					mInitiialized = false;
	bool					mRerouteRequired = false;

	void					AddOre(float newOre) { mCollectedOre += newOre; }


public:

	MiningSimulation();

	bool		Execute();

	int			AllocIdleMine();
	
	Station*	AllocIdleStation();

	std::vector<Mission*>	GetActiveMissions() { return mActiveMissions; }

protected:
	virtual ~MiningSimulation();


	void Init();

	void BuildLists();
	void AdjustInputValues();
	void PopulateStations();
	void DoRoutingLogic();
	void StartMission(int truckID,int mineID, Station* station);
	void CleanMissions();

	void PrintElapsedTime(const char* label);
	void PrintBanner();
	void ShowStations();
	


};
