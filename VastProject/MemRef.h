#pragma once
#include <list>
class MemRef
{

private:

	static std::list<MemRef*>   _sAllReferences;
public:
	MemRef();

	virtual ~MemRef();
	

	static void Finish();


};