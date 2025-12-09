#include "Instance.h"
#include <memory>

class QuadPart : public Instance {
public:
	QuadPart() = default;
	QuadPart(const QuadPart&) = default;
	~QuadPart() override = default;

	std::unique_ptr<Instance> clone() const override {
		return std::make_unique<QuadPart>(*this);
	}
};