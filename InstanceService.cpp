#include "InstanceService.h"

InstanceService::InterfacePtrRef InstanceService::get()
{
	static InstanceService::InterfacePtrRef instance(new InstanceService);
	return instance;
}

//InstanceService::~InstanceService()
//{
//	std::cout << "InstanceService ptr was culled\n";
//}

InstanceService::InstanceService() {
	std::cout << "Instance Service was constructed\n";
}
