#include <iostream>
#include "SimDefinitions.h"
#include "Station.h"
#include "Truck.h"

int  Station::_sStationIDCount = 0;

Station::Station()
{
    mIdleTrucks.clear();

    mID = _sStationIDCount++;
}

Station::~Station()
{
}

void Station::AddIdleTruck(int truck)
{
    // Filter out bad trucks. SHould be an assert here
    if (truck < 0)
        return;

    auto it = std::find(mIdleTrucks.begin(), mIdleTrucks.end(), truck);

    if (it == mIdleTrucks.end())
        mIdleTrucks.push_back(truck);

}


int Station::RemoveIdleTruck()
{
    if (mIdleTrucks.size() < 1)
        return -1;

    int retVal = mIdleTrucks.front();

    mIdleTrucks.erase(mIdleTrucks.begin());

    return retVal;
}

// Debugging only
void Station::Report()
{
    std::cout << "Stationed Trucks: ";

    for(auto truckIter:mIdleTrucks)
        std::cout << truckIter << " ";

    std::cout << std::endl;

}
