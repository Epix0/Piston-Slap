#pragma once
#include <mutex>
#include <memory>
// The InstanceService class is a major Singleton service and library which wrapped constructors for game objects to handle mem
class InstanceService
{
public:
	InstanceService* get();
private:
	static std::mutex initLock;
	InstanceService();
	InstanceService* singletonPtr;
};

