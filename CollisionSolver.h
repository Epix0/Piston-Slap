#pragma once
#include "Mesh.h"
#include <vector>
#include <limits>
#include "Instance.h"

class CollisionSolver {
	struct MinMax {
		float mMin;
		float mMax;

		MinMax() : mMin(std::numeric_limits<float>::max()), mMax(std::numeric_limits<float>::min()) {};
	};

public:

	CollisionSolver() {};
	~CollisionSolver() = default;

	static bool areTheseInstancesTouching(std::shared_ptr<Instance> instanceA, std::shared_ptr<Instance> instanceB);

	static struct MinMax projectVertices(const std::vector<Mesh::Vertex>& vertices, const glm::vec3& axis, std::shared_ptr<Instance> instance);
};