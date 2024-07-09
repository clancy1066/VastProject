
#include <iostream>
#include "ClassFactory.h"

ClassFactory::ClassFactory()
{
	_sAllReferences.push_back(this);
}

ClassFactory::~ClassFactory()
{
	std::cout << "Destructor of ClassFactory" << std::endl;
}

template<typename T>
T* ClassFactory::Create(const std::string& className)
{
    template<typename T>
    T* CreateInstance(const std::string & className) {
        auto it = classTypes.find(className);
        if (it != classTypes.end()) {
            return new T();
        }
        else {
            throw std::runtime_error("Class %s not found",className);
        }
    }

}

