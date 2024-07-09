#pragma once


class Truck
{
public:
	Truck();

	~Truck();

	bool	Execute(float dt);

	void	StartMission(float waitTime,float loadingTime);
	void	ReturnToStation();

	int		GetID() { return mID; }

protected:

	


private:

	float mTaskTime = 0;

	static int  _sTruckIDCount;
	int			mID = 0;


};