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

using std::mutex, std::string;

void CustomModelImporter::ProcessNode(const aiNode* node, Model& model, const aiScene* scene) const {
    if (node->mNumChildren > 0) {
        for (size_t i = 0; i < node->mNumChildren; ++i) {
            ProcessNode(node->mChildren[i], model, scene);
        }
    }

    for (unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex) {
        // This is the scene's mesh
        aiMesh* sceneMeshChild = scene->mMeshes[meshIndex];

        // This is my mesh
        Mesh& meshOfModel = model.mMeshes.emplace_back();

        meshOfModel.mVertexData.reserve(sceneMeshChild->mNumVertices);
        meshOfModel.mElements.reserve(static_cast<size_t>(sceneMeshChild->mNumFaces) * 3); // post proc flags forces trianulation, so it's safe to assume 3

        // Elements
        aiFace* facesArray = sceneMeshChild->mFaces;
        for (size_t i = 0; i < sceneMeshChild->mNumFaces; ++i) {
            auto& face = facesArray[i];
            auto* indicesArray = face.mIndices;

            for (size_t j = 0; j < face.mNumIndices; ++j) {
                meshOfModel.mElements.push_back(indicesArray[j]);
            }
        }

        // TexCoords. While the iteration len is mNumVertices, only the first (0) set is used
        auto ptrTexCoordsArray = sceneMeshChild->mTextureCoords[0]; // the first set only

         // Vertices and Normals
        for (unsigned int vertexIndex = 0; vertexIndex < sceneMeshChild->mNumVertices; vertexIndex++) {
            Vertex vertex{};

            auto& positionVertex = sceneMeshChild->mVertices[vertexIndex];
            auto& normal = sceneMeshChild->mNormals[vertexIndex];
            vertex.Position = glm::vec3(positionVertex.x, positionVertex.y, positionVertex.z);
            vertex.Normal = glm::vec3(normal.x, normal.y, normal.z);
            
            // TexCoords is an array itself, check if there's validation first
            if (ptrTexCoordsArray) {
                for (unsigned int i = 0; i < sceneMeshChild->mNumVertices; ++i) {
                    auto& uvCoords = ptrTexCoordsArray[i];
                    vertex.TexCoords = glm::vec2(uvCoords.x, uvCoords.y);
                }
            } else {
                vertex.TexCoords = glm::vec2(0);
            }
            
            meshOfModel.mVertexData.push_back(vertex);
        }
       
        // Mesh finalization
        meshOfModel.PrepareForGL();
    }
}

void CustomModelImporter::ProcessScene(const aiScene* scene, std::string&& modelName) {
    Model& model = mImportedModels.emplace_back();
    model.mName = modelName;
    model.mMeshes.reserve(scene->mNumMeshes);

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
