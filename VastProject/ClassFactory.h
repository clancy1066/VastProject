#pragma once
#include <iostream>
#include <string>
#include <typeindex>
#include <memory>
#include <unordered_map>


#include "Truck.h"
#include "Mine.h"
#include "Station.h"

template<typename T>
T* CreateInstance(const std::string& className);

class ClassFactory
{

private:

	std::unordered_map<std::string, std::type_index> classTypes = {
	{
			"Truck", typeid(Truck)}
	};

public:
	ClassFactory();

	virtual ~ClassFactory();
	
	template<typename T>
	T* CreateInstance(const std::string& className);


};