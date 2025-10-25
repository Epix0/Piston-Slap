#include <iostream>
#include <memory>
#include "PhysicsObject.h"
int main() {
	std::unique_ptr<int> four = std::make_unique<int>(4);

	std::cout << *four << "\n";

	return 0;
}