#pragma once
#include <vector>

// *********************************************************
// Stations can hoave 1 or more trucks waiting for missions 
// *********************************************************
class Station
{
public:
	Station();

	~Station();

	int GetID() { return mID; }

	int		GetNumIdleTrucks()	{ return (int)mIdleTrucks.size(); };

	void	AddIdleTruck(int truck);
	int		RemoveIdleTruck();

	// Debugging
	void	Report();
	
protected:

	float mOreCollected = 0;

	std::vector<int>	mIdleTrucks;

protected:

	static int  _sStationIDCount;
	int			mID;


private:

};