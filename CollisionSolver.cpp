#include "CollisionSolver.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.inl"
#include <vector>
#include "Instance.h"
#include <memory>

bool CollisionSolver::areTheseInstancesTouching(std::shared_ptr<Instance> instanceA, std::shared_ptr<Instance> instanceB) {
    auto& meshA = instanceA->getModel()->mMeshes[0];
    auto& meshB = instanceB->getModel()->mMeshes[0];

    for(int i = 0; i < meshA.mVertices.size(); ++i) {
        auto vertexA = glm::vec3(instanceA->getTransform() * glm::vec4(meshA.mVertices[i].Position, 1.f));
        auto vertexB = glm::vec3(instanceA->getTransform() * glm::vec4(meshA.mVertices[(i + 1) % meshA.mVertices.size()].Position, 1.f));

        glm::vec3 edge = vertexB - vertexA;
        glm::vec3 axis = glm::normalize(meshA.mVertices[(i + 1) % meshA.mVertices.size()].Normal - meshA.mVertices[i].Normal);

        auto minMaxA = projectVertices(meshA.mVertices, axis, instanceA);
        auto minMaxB = projectVertices(meshB.mVertices, axis, instanceB);

        if(minMaxA.mMin >= minMaxB.mMax || minMaxB.mMin >= minMaxA.mMax)
            return false;
    }

    return true;
}

CollisionSolver::MinMax CollisionSolver::projectVertices(const std::vector<Mesh::Vertex>& vertices, const glm::vec3& axis, std::shared_ptr<Instance> instance) {
    auto aabb = CollisionSolver::MinMax{};

    for(int i = 0; i < vertices.size(); ++i) {
        auto vertex = glm::vec3(instance->getTransform() * glm::vec4(vertices[i].Position, 1.f));
        float projection = glm::dot(vertex, axis);

        if(projection < aabb.mMin)
            aabb.mMin = projection;
        if(projection > aabb.mMax)
            aabb.mMax = projection;
    }

    return aabb;
}

