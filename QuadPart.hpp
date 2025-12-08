#include "Instance.h"
#include <string>
#include <memory>

class QuadPart : public Instance {
public:
	QuadPart() = default;
	QuadPart(const QuadPart&) = default;

	std::unique_ptr<Instance> clone() const override {
		return std::make_unique<QuadPart>();
	}
private:

};