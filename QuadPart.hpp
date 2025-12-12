#include "Instance.h"
#include <memory>
#include <iostream>

class QuadPart : public Instance {
public:
	QuadPart() = default;
	QuadPart(const QuadPart&) = default;
	~QuadPart() override {
		std::cout << "A QuadPart base was destroyed\n";
	};

	std::unique_ptr<Instance> clone() const override {
		return std::make_unique<QuadPart>(*this);
	}
};