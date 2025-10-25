#pragma once
#include <mutex>
#include <memory>
#include <iostream>
// The InstanceService class is a major Singleton service and library which wrapped constructors for game objects to handle mem
class InstanceService
{
public:
	using InterfacePtrRef = std::shared_ptr<InstanceService>;
	static InterfacePtrRef get();

	//~InstanceService();
private:
	InstanceService();
};

