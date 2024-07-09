
#include <iostream>
#include "ClassFactory.h"

ClassFactory::ClassFactory()
{

}

ClassFactory::~ClassFactory()
{
	std::cout << "Destructor of ClassFactory" << std::endl;
}




template<typename T>
T* CreateInstance(const std::string& className)
{
    return NULL;
}

