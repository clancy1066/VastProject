#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <string>

//#include "C:/Users/clanc/AppData/Local/simple2d/simple2d.h"

#include "MiningSimulation.h"

int  MiningSimulation::_sMineIDCount = 0;
int  MiningSimulation::_sTruckIDCount = 0;

#include "Truck.h"
#include "Mine.h"
#include "Station.h"

MiningSimulation::MiningSimulation()
{
	Init();
}


MiningSimulation::~MiningSimulation()
{
}

void MiningSimulation::Init()
{
	system("cls"); // Clears the console screen


	int totalProspective = (int) (SIM_MAX_TIME_IN_SECONDS_F / SIM_MAX_MISSION_IN_SECONDS_F);

	std::cout << "Total prospective ore = " << totalProspective*5 << std::endl;


	PrintBanner();

	QueryParameters();

	AdjustInputValues();

	BuildLists();

	DoRoutingLogic();
}

void MiningSimulation::BuildLists()
{
	mRerouteRequired = true;

	mAllMines.clear();
	mAllStations.clear();

	mInitiialized = (mNumTrucks > 0 && mNumMines > 0 && mNumStations > 0);


	if (!mInitiialized)
		return;

	
	std::cout << "Adjusted values: Num stations " << mNumStations << " Num Trucks: " << mNumTrucks << "  Num Mines: " << mNumMines << std::endl;

	for (int mineIter = 0; mineIter < mNumMines; mineIter++)
	{
		int mine = _sMineIDCount++;

		mAllMines.push_back(mine);
	}

	for (int stationIter = 0; stationIter < mNumStations; stationIter++)
	{
		Station* station = new Station();

		std::cout << "Added station " << station->GetID() << std::endl;

		mAllStations.push_back(station);
	}

	PopulateStations();
// For debugging 	ShowStations();
}

void MiningSimulation::AdjustInputValues()
{
	// Never need more than 2 trucks per mine
	if (mNumTrucks > (mNumMines * 2))
		mNumTrucks = (mNumMines * 2);

	// Only need enough mines for truck amount
	if (mNumMines > mNumTrucks)
	{
		mNumMines = mNumTrucks;
	}

}

// *********************************************************
// Stations can hoave 1 or more trucks waiting for missions 
// This is where the trucks get asigned to stations
// *********************************************************
void MiningSimulation::PopulateStations()
{
	int numTrucksPerStation			= mNumTrucks / mNumStations;
	int effectiveTrucksPerStation	= numTrucksPerStation * mNumStations;
	int extraTrucks					= mNumTrucks - effectiveTrucksPerStation;


	for (auto stationIter : mAllStations)
	{
		int truckID = _sTruckIDCount++;

		for (int i = 0; i < numTrucksPerStation; i++)
		{
			stationIter->AddIdleTruck(truckID);
			truckID = _sTruckIDCount++;
		}
		

		if (extraTrucks > 0)
		{
			truckID = _sTruckIDCount++;;

			stationIter->AddIdleTruck(truckID);

			extraTrucks--;

		}
	}
	std::cout << "Num trucks per station " << numTrucksPerStation << " Minimal trucks " << effectiveTrucksPerStation << " Extra trucks " << extraTrucks << std::endl;
}

// ***********************************************
// This is what invokes new missions
// ***********************************************
void MiningSimulation::DoRoutingLogic()
{
	if (!mRerouteRequired)
		return;

	mRerouteRequired = false;

	float maxVal = SIM_MAX_TIME_IN_SECONDS_F;
	float remainingTime = maxVal - mElapsedTime;

	// CHeck to see if there is time for another mission
	if (remainingTime < SIM_MAX_MISSION_IN_SECONDS_F)
	{
		// No more new missions
		return;
	}

	if ((mAllMines.size() < 1) || (mAllStations.size()<1))
		return;


	for (auto stationIter : mAllStations)
	{
		if (stationIter->GetNumIdleTrucks()<1)
			continue;

		std::cout << "Processing Mission for station " << stationIter->GetID() << std::endl;
		int truckID = stationIter->RemoveIdleTruck();
		int lastMineID = -1;

		while (truckID >= 0)
		{
			int mineID = AllocIdleMine();

			if (mineID > -1)
			{
				StartMission(truckID, mineID, stationIter);
			}
			else
			{
				std::cout << "Yhis should never happen" << std::endl;
			}
			truckID = stationIter->RemoveIdleTruck();

			lastMineID = mineID;
		}

	}
	
	float truckTime = remainingTime;

	if (remainingTime >= SIM_MAX_MISSION_IN_SECONDS_F)
	{
		truckTime = SIM_MAX_MISSION_IN_SECONDS_F;
	}

}

void MiningSimulation::StartMission(int truckID, int mineID, Station* station)
{
	if ((truckID < 0) || (mineID < 0) || (station == NULL))
		return;



	PrintElapsedTime(" Starting New Mission: ");

	Mission* newMission = AllocMission();

	newMission->Begin(truckID, mineID, station, mElapsedTime);

	mActiveMissions.push_back(newMission);

}

Mission* MiningSimulation::AllocMission()
{
	Mission* newMission = nullptr;
	
	if (mFreeMissionPool.size() > 0)
	{
		newMission = mFreeMissionPool.back();
		mFreeMissionPool.pop_back();
	}
	else
		newMission = new Mission();

	return newMission;
}

void MiningSimulation::FreeMission(Mission*oldMission)
{
	if (oldMission == nullptr)
		return;

	mFreeMissionPool.push_back(oldMission);
}

void MiningSimulation::EndMission(Mission* mission)
{
	if (mission == nullptr)
		return;

	mCollectedOre += mission->GetCollectedOre();

	std::cout << "End mission collects: " << mCollectedOre << " ore." << std::endl;

	mMissionsToRemove.push_back(mission);
}

void MiningSimulation::QueryParameters()
{
	std::cout << " Number of Trucks: ";
	std::cin >> mNumTrucks;

	std::cout << " Number of Mines: ";
	std::cin >> mNumMines;

	std::cout << " Number of Stations: ";
	std::cin >> mNumStations;
}


void MiningSimulation::CleanMissions()
{
	for (auto missionIter : mMissionsToRemove)
	{
		Station* station = missionIter->GetStation();

		mRerouteRequired = true;

		station->AddIdleTruck(missionIter->GetTruckID());

		auto it = std::find(mActiveMissions.begin(), mActiveMissions.end(), missionIter);


		if (it != mActiveMissions.end())
			mActiveMissions.erase(it);

		FreeMission(missionIter);
	}

	mMissionsToRemove.clear();
}



void MiningSimulation::PrintElapsedTime(const char* label=nullptr)
{
	if (label)
		std::cout << label;

	int hours = (int)(mElapsedTime / (3600.0f));
	int mins = (int)(mElapsedTime / (60.0f))%60;
	int secs = (int)(mElapsedTime) % 60;

	std::cout << "Time " << hours << " hours. " << mins << " mins." << secs << " seconds" << std::endl;
}

void MiningSimulation::PrintBanner()
{
	std::cout << "Rules" << std::endl;

	std::cout << "- The total simulation time is " << SIM_MAX_TIME_IN_HOURS_F << " hours." << std::endl;
	std::cout << "- Travelling time from a staion to a mine is " << SIM_TRAVEL_TIME_IN_MINUTES_F << " minutes." << std::endl;
	std::cout << "- A truck can stay at a mine for a minimum of" << SIM_MIN_EXTRACT_TIME_IN_HOURS_F << " hour and and maximum of " << SIM_MAX_EXTRACT_TIME_IN_HOURS_F << " hours." << std::endl;
	std::cout << "- A truck takes " << SIM_WAIT_UNLOAD_IN_MINUTES_F << " minutes to unload ore at a station." << std::endl << std::endl;

}

void MiningSimulation::ShowStations()
{
	for (auto stationIter : mAllStations)
		stationIter->Report();
}


// *****************************************
// Returns true if it it is done processing
// *****************************************
bool MiningSimulation::Execute()
{
	if (!mInitiialized)
		return true;

	if (mActiveMissions.size() < 1)
		return false;

	float maxVal = SIM_MAX_MISSION_IN_SECONDS_F / 3600.0f;

	mElapsedTime += mDeltaTime ;

	//std::cout << "mElapsedTime " << mElapsedTime/3600 << " in hours." << std::endl;

//	if (mElapsedTime >= mSimulationTime)
//		return true;

	for (auto missionIter : mActiveMissions)
	{
		// Returns true when done
		if (missionIter->Execute(mDeltaTime))
		{
			EndMission(missionIter);
		}
		else
		{
			if (missionIter->CanFreeMine())
			{
				int mineToFree = missionIter->GetMineID();

				mAllMines.push_back(mineToFree);
				missionIter->FreeMine();

				mRerouteRequired = true;
			}

		}
	}

	if (mElapsedTime >= SIM_MAX_TIME_IN_SECONDS_F)
	{
		std::cout << "Similation ends at " << mElapsedTime /(60.0f*60.0f) << " hours." << std::endl;
		std::cout << "Collected " << mCollectedOre << " ore." << std::endl;
		
		return true;
	}

	CleanMissions();

	DoRoutingLogic();

	return false;
}


int MiningSimulation::AllocIdleMine()
{
	if (mAllMines.size() < 1)
		return -1;

	int mine = mAllMines.front();

	mAllMines.erase(mAllMines.begin());

	return mine;
}

Station* MiningSimulation::AllocIdleStation()
{
	if (mAllStations.size() < 1)
		return nullptr;

	Station* station = mAllStations.front();

	mAllStations.erase(mAllStations.begin());

	return station;
}

