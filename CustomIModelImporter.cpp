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
    for (unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex) {
        // This is the scene's mesh
        aiMesh& sceneMeshChild = *scene->mMeshes[node->mMeshes[meshIndex]];
        std::cout << sceneMeshChild.mName.C_Str() << "\n";
        // This is my mesh
        Mesh& meshOfModel = model.mMeshes.emplace_back();

        meshOfModel.mVertexData.reserve(sceneMeshChild.mNumVertices);
        meshOfModel.mElements.reserve(static_cast<size_t>(sceneMeshChild.mNumFaces) * 3); // post proc flags forces trianulation, so it's safe to assume 3

        // Elements
        aiFace* facesArray = sceneMeshChild.mFaces;
        for (size_t i = 0; i < sceneMeshChild.mNumFaces; ++i) {
            auto& face = facesArray[i];
            auto* indicesArray = face.mIndices;

            for (size_t j = 0; j < face.mNumIndices; ++j) {
                meshOfModel.mElements.push_back(indicesArray[j]);
            }
        }

        // TexCoords. While the iteration len is mNumVertices, only the first (0) set is used
        auto ptrTexCoordsArray = sceneMeshChild.mTextureCoords[0]; // the first set only

         // Vertices and Normals
        for (unsigned int vertexIndex = 0; vertexIndex < sceneMeshChild.mNumVertices; vertexIndex++) {
            Vertex vertex{};

            auto& positionVertex = sceneMeshChild.mVertices[vertexIndex];
            auto& normal = sceneMeshChild.mNormals[vertexIndex];
            vertex.Position = glm::vec3(positionVertex.x, positionVertex.y, positionVertex.z);
            vertex.Normal = glm::vec3(normal.x, normal.y, normal.z);
            
            // TexCoords is an array itself, check if there's validation first
            if (ptrTexCoordsArray) {
                for (unsigned int i = 0; i < sceneMeshChild.mNumVertices; ++i) {
                    auto& uvCoords = ptrTexCoordsArray[i];
                    vertex.TexCoords = glm::vec2(uvCoords.x, uvCoords.y);
                }
            } else {
                vertex.TexCoords = glm::vec2(0);
            }
            // DONE with Vertex
            meshOfModel.mVertexData.push_back(vertex);


#if false
            // TEXTURES AKA MATERIALS
            aiMaterial* material = scene->mMaterials[sceneMeshChild->mMaterialIndex];
            // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
            // Same applies to other texture as the following list summarizes:
            // diffuse: texture_diffuseN
            // specular: texture_specularN
            // normal: texture_normalN

            // 1. diffuse maps
            vector<Texture> diffuseMaps = model.loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            meshOfModel.mTextureData.insert(meshOfModel.mTextureData.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            vector<Texture> specularMaps = model.loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            meshOfModel.mTextureData.insert(meshOfModel.mTextureData.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Texture> normalMaps = model.loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            meshOfModel.mTextureData.insert(meshOfModel.mTextureData.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            std::vector<Texture> heightMaps = model.loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            meshOfModel.mTextureData.insert(meshOfModel.mTextureData.end(), heightMaps.begin(), heightMaps.end());
#endif
        }
        // Mesh finalization
        meshOfModel.PrepareForGL();
    }

    if (node->mNumChildren > 0)
    {
        for (size_t i = 0; i < node->mNumChildren; ++i)
        {
            ProcessNode(node->mChildren[i], model, scene);
        }
    }
}

void CustomModelImporter::ProcessScene(const aiScene* scene, std::string&& modelName) {
    Model& model = mImportedModels.emplace_back();
    model.mName = modelName;
    model.mMeshes.reserve(scene->mNumMeshes);
    
    glm::mat4 rootTransform = glm::mat4(0);

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j){
            rootTransform[i][j] = scene->mRootNode->mTransformation[i][j];
        }
    }

    ProcessNode(scene->mRootNode, model, scene);

   /* for (unsigned int childNodeIndex = 0; childNodeIndex < scene->mRootNode->mNumChildren; ++childNodeIndex) {
        ProcessNode(scene->mRootNode->mChildren[childNodeIndex], model, scene);
    }*/
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
        aiProcess_Triangulate);

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
