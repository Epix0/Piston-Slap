#include "InstanceService.h"

#include <mutex>
#include <thread>
InstanceService* InstanceService::get()
{
	if (this->singletonPtr == nullptr) {
		std::lock_guard<std::mutex> lock(this->initLock);
	
	}
	
	return nullptr;
}
