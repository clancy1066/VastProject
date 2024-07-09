#pragma once

class Mine
{
public:
	Mine();

	~Mine();

	int		GetID() { return mID; }

private:

	static int  _sMineIDCount;
	int			mID = 0;
};