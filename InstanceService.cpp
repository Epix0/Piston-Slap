#include "InstanceService.h"

std::unique_ptr<InstanceService>& InstanceService::get()
{
	if (this->singletonPtr == nullptr) {
		std::lock_guard<std::mutex> lock(this->initLock);
		if (this->singletonPtr == nullptr) {
			static InstanceService* localInstance = new InstanceService;
			this->singletonPtr = std::make_unique<InstanceService>();
		}
	}
	
	return this->singletonPtr;
}

InstanceService::InstanceService() {
	std::cout << "Instance Service was init\n";
}
