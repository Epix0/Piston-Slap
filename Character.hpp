#include "Instance.h"

class Character : public Instance {
public:

	std::unique_ptr<Instance> clone() const override{
		return std::make_unique<Character>(*this);
	}
};