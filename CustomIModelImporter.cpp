#include "CustomIModelImporter.h"
#include <string>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <iostream>
#include <filesystem>
#include <mutex>

using std::mutex;

void CustomModelImporter::ProcessNode(const aiNode* node, Model& model, const aiScene* scene) const {
    if (node->mNumChildren > 0) {
        for (size_t i = 0; i < node->mNumChildren; ++i) {
            ProcessNode(node->mChildren[i], model, scene);
        }
    }

    for (unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex) {
        aiMesh* sceneMeshChild = scene->mMeshes[meshIndex];

        Mesh& meshOfModel = model.mMeshes.emplace_back(sceneMeshChild->mName.C_Str());

        meshOfModel.mVertices.reserve(sceneMeshChild->mNumVertices);
        meshOfModel.mNormals.reserve(sceneMeshChild->mNumVertices);
        meshOfModel.mElements.reserve(static_cast<size_t>(sceneMeshChild->mNumFaces) * 3);

        // Elements
        aiFace* facesArray = sceneMeshChild->mFaces;
        for (size_t i = 0; i < sceneMeshChild->mNumFaces; ++i) {
            auto& face = facesArray[i];
            auto* indicesArray = face.mIndices;

            for (size_t j = 0; j < face.mNumIndices; ++j) {
                meshOfModel.mElements.push_back(indicesArray[j]);
            }
        }

        // Vertices and Normals
        for (unsigned int vertexIndex = 0; vertexIndex < sceneMeshChild->mNumVertices; vertexIndex++) {
            auto& vertex = sceneMeshChild->mVertices[vertexIndex];
            auto& normal = sceneMeshChild->mNormals[vertexIndex];
            meshOfModel.mVertices.emplace_back(static_cast<float>(vertex.x), static_cast<float>(vertex.y), static_cast<float>(vertex.z));
            meshOfModel.mNormals.emplace_back(static_cast<float>(normal.x), static_cast<float>(normal.y), static_cast<float>(normal.z));
        }
    }
}

void CustomModelImporter::ProcessScene(const aiScene* scene, std::string&& modelName) {
    Model& model = mImportedModels.emplace_back();
    model.mName = modelName;
    model.mMeshes.reserve(scene->mNumMeshes);
    model.mNumOfMeshes = scene->mNumMeshes;

    for (unsigned int childNodeIndex = 0; childNodeIndex < scene->mRootNode->mNumChildren; ++childNodeIndex) {
        ProcessNode(scene->mRootNode->mChildren[childNodeIndex], model, scene);
    }
}

bool CustomModelImporter::ImportModelFile(const std::string& pFile) {
    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(pFile,
        aiProcess_SortByPType|
        aiProcess_JoinIdenticalVertices |
        aiProcess_Triangulate|
        aiProcess_FindDegenerates|
        aiProcess_FixInfacingNormals);

    // If the import failed, report it
    if (nullptr == scene) {
        std::cout << "ImportModelFile() failed: " << (importer.GetErrorString()) << "\n";
        return false;
    }

    // Now we can access the file's contents.
    ProcessScene(scene, std::filesystem::path(pFile).filename().string());

    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}
