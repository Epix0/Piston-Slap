#include "CustomIModelImporter.h"
#include <string>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <iostream>
#include <filesystem>

// TODO: need to create a function to handle child nodes OF nodes. Currently only supports single-layered nodes from the RootNode
void CustomModelImporter::ProcessScene(const aiScene* scene, std::string&& modelName) {
    Model& model = mImportedModels.emplace_back();
    model.mName = modelName;
    model.mMeshes.reserve(scene->mNumMeshes);

    /*
    Create Model ->
    From the rootnode, go through each node->
    With each node, go through all its meshes ->
        -> Through each mesh instance, create a Mesh obj and add it to the Model
    */

    for (unsigned int childNodeIndex = 0; childNodeIndex < scene->mRootNode->mNumChildren; ++childNodeIndex) {
        aiNode* node = scene->mRootNode->mChildren[childNodeIndex];
        for (unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex) {
            aiMesh* sceneMeshChild = scene->mMeshes[meshIndex];
            
            Mesh& meshOfModel = model.mMeshes.emplace_back(sceneMeshChild->mName.C_Str());

            meshOfModel.mVertices.reserve(sceneMeshChild->mNumVertices);
            meshOfModel.mNormals.reserve(sceneMeshChild->mNumVertices);

            for (unsigned int vertexIndex = 0; vertexIndex < sceneMeshChild->mNumVertices; vertexIndex++) {
                auto& vertex = sceneMeshChild->mVertices[vertexIndex];
                auto& normal = sceneMeshChild->mNormals[vertexIndex];
                meshOfModel.mVertices.emplace_back(static_cast<float>(vertex.x), static_cast<float>(vertex.y), static_cast<float>(vertex.z));
                meshOfModel.mVertices.emplace_back(static_cast<float>(normal.x), static_cast<float>(normal.y), static_cast<float>(normal.y));
            }
        }
    }
}

bool CustomModelImporter::ImportModelFile(const std::string& pFile) {
    // Create an instance of the Importer class
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(pFile,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

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
