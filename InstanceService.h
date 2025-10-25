#pragma once
#include <mutex>
#include <memory>
#include <iostream>
// The InstanceService class is a major Singleton service and library which wrapped constructors for game objects to handle mem
class InstanceService
{
public:
	std::unique_ptr<InstanceService>& get();
private:
	std::mutex initLock;
	InstanceService();
	std::unique_ptr<InstanceService> singletonPtr;
};

