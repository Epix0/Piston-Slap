#include "CustomIModelImporter.h"
#include <string>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <iostream>
#include <filesystem>
#include <mutex>
#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "glm/fwd.hpp"
#include "Mesh.h"
#include "Model.h"

using std::string;

void CustomModelImporter::processVertices(const aiMesh& sceneMesh, Mesh& meshOfModel) const {
    meshOfModel.mVertices.reserve(sceneMesh.mNumVertices);

    // TexCoords. While the iteration len is mNumVertices, only the first (0) set is used
    auto ptrTexCoordsArray = sceneMesh.mTextureCoords[0]; // the first set only
    
    for (unsigned int vertexIndex = 0; vertexIndex < sceneMesh.mNumVertices; ++vertexIndex)
    {
        Vertex vertex;
        auto& positionVertex = sceneMesh.mVertices[vertexIndex];
        auto& normal = sceneMesh.mNormals[vertexIndex];

        vertex.Position = glm::vec3(positionVertex.x, positionVertex.y, positionVertex.z);
        vertex.Normal = glm::vec3(normal.x, normal.y, normal.z);

        // TexCoords is an array itself, check if there's validation first
        if (ptrTexCoordsArray)
        {
            auto& uvCoords = ptrTexCoordsArray[vertexIndex];
            vertex.TexCoords = glm::vec2(uvCoords.x, uvCoords.y);
        }
        else
        {
            vertex.TexCoords = glm::vec2(0);
        }
        // DONE with Vertex

        meshOfModel.mVertices.push_back(vertex);
    }
}

void CustomModelImporter::proccessElements(const aiMesh& sceneMesh, Mesh& meshOfModel) const {
    meshOfModel.mElements.reserve(static_cast<size_t>(sceneMesh.mNumFaces) * 3); // post proc flags forces trianulation, so it's safe to assume 3

    aiFace* facesArray = sceneMesh.mFaces;

    for (size_t i = 0; i < sceneMesh.mNumFaces; ++i) {
        auto& face = facesArray[i];
        auto* indicesArray = face.mIndices;

        for (size_t j = 0; j < face.mNumIndices; ++j) {
            meshOfModel.mElements.push_back(indicesArray[j]);
        }
    }
}

void CustomModelImporter::processAIMesh(const aiMesh& sceneMesh, Model& parentModel) const {
    // This is my mesh
    Mesh& meshOfModel = parentModel.mMeshes.emplace_back();

    processVertices(sceneMesh, meshOfModel);
    proccessElements(sceneMesh, meshOfModel);
    meshOfModel.prepareForGL();
}

void CustomModelImporter::processNodeRecursively(const aiNode* node, Model& model, const aiScene* scene) const {
    for (unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex) {
        processAIMesh(*scene->mMeshes[node->mMeshes[meshIndex]], model);
    }

    if (node->mNumChildren > 0) {
        for (size_t i = 0; i < node->mNumChildren; ++i) {
            processNodeRecursively(node->mChildren[i], model, scene);
        }
    }
}

void CustomModelImporter::processScene(const aiScene* scene, std::string&& modelName) {
    Model& model = mImportedModels.emplace_back();
    model.mName = modelName;
    model.mMeshes.reserve(scene->mNumMeshes);

    processNodeRecursively(scene->mRootNode, model, scene);
}

bool CustomModelImporter::ImportModelFile(const std::string& pFile) {
    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(pFile,
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate| 
        aiProcess_OptimizeMeshes|
        aiProcess_PreTransformVertices);

    // If the import failed, report it
    if (nullptr == scene) {
        std::cout << "ImportModelFile() failed: " << (importer.GetErrorString()) << "\n";
        return false;
    }

    // Now we can access the file's contents.
    processScene(scene, std::filesystem::path(pFile).filename().string());

   // We're done. Everything will be cleaned up by the importer destructor
    return true;
}