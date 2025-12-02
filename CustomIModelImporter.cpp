#include "CustomIModelImporter.h"
#include <string>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <filesystem>
#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "glm/glm.hpp"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "assimp/material.h"
#include "assimp/types.h"
#include <memory>
#include <utility>

// post proc flags
constexpr unsigned int cPostProcFlags =

aiProcess_JoinIdenticalVertices |
aiProcess_Triangulate |
aiProcess_OptimizeMeshes |
aiProcess_PreTransformVertices;

// Quick note: processVertices() handles the texture coords. This function handles the import of pixel data

void CustomModelImporter::processTextures(const aiMesh& sceneMesh, Mesh& meshOfModel, const aiScene* scene, const std::string& modelName) const {
    auto& material = *scene->mMaterials[sceneMesh.mMaterialIndex];

    aiTextureType textureType = aiTextureType_NONE;

    if(material.GetTextureCount(aiTextureType_DIFFUSE) > 0)
        textureType = aiTextureType_DIFFUSE;
    else return;

    aiString aiPath;
    material.GetTexture(textureType, 0, &aiPath); // index = 0 :: ONLY taking the first texture set

    std::string strPath = aiPath.C_Str();
    std::string filename = strPath.substr(strPath.find_last_of("/\\") + 1);
   
    meshOfModel.mTexture = std::make_shared<Texture>(filename.c_str(), modelName);
    if(meshOfModel.mTexture->getGLTextureId() == 0) {
        std::cout << "Skipping " << filename << "\n";
        meshOfModel.mTexture.reset();
        meshOfModel.mTexture = nullptr;
    }
}

// Currently handles: Positions, Normals, TexCoords, MaterialColors
void CustomModelImporter::processVertices(const aiMesh& sceneMesh, Mesh& meshOfModel, const aiScene* scene) const {
    meshOfModel.mVertices.reserve(sceneMesh.mNumVertices);

    // Vars used for potential nullptr values
    // TexCoords. While the iteration len is mNumVertices, only the first (0) set is used
    auto ptrTexCoordsArray = sceneMesh.mTextureCoords[0];
    auto ptrColor = sceneMesh.mColors[0];

    for (unsigned int vertexIndex = 0; vertexIndex < sceneMesh.mNumVertices; ++vertexIndex)
    {
        Vertex vertex;
        auto& positionVertex = sceneMesh.mVertices[vertexIndex];
        auto& normal = sceneMesh.mNormals[vertexIndex];

        vertex.Position = glm::vec3(positionVertex.x, positionVertex.y, positionVertex.z);
        vertex.Normal = glm::vec3(normal.x, normal.y, normal.z);

        // TexCoords and Colors may not exist. Guard clauses are set

        // TexCoords
        if (ptrTexCoordsArray) {
            auto& uvCoords = ptrTexCoordsArray[vertexIndex];

            vertex.TexCoords = glm::vec2(uvCoords.x, uvCoords.y);
    
        } else {
            vertex.TexCoords = glm::vec2(0);
        }

        // Color
        auto material = scene->mMaterials[sceneMesh.mMaterialIndex];
        aiColor3D aiColor(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
        vertex.Color = glm::vec3(aiColor.r, aiColor.g, aiColor.b);

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

void CustomModelImporter::processAIMesh(const aiMesh& sceneMesh, Model& parentModel, const aiScene* scene, const std::string& modelName) const {
    // This is my mesh
    Mesh& meshOfModel = parentModel.mMeshes.emplace_back();

    processVertices(sceneMesh, meshOfModel, scene);
    proccessElements(sceneMesh, meshOfModel);
    processTextures(sceneMesh, meshOfModel, scene, modelName);

    meshOfModel.prepareForGL();
}

void CustomModelImporter::processNodeRecursively(const aiNode* node, Model& model, const aiScene* scene, const std::string& modelName) const {
    for (unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex) {
        processAIMesh(*scene->mMeshes[node->mMeshes[meshIndex]], model, scene, modelName);
    }

    if (node->mNumChildren > 0) {
        for (size_t i = 0; i < node->mNumChildren; ++i) {
            processNodeRecursively(node->mChildren[i], model, scene, modelName);
        }
    }
}

void CustomModelImporter::processScene(const aiScene* scene, const std::string& modelName) {
    auto& model = *mImportedModels[modelName];
    model.mMeshes.reserve(scene->mNumMeshes);

    processNodeRecursively(scene->mRootNode, model, scene, modelName);
}

bool CustomModelImporter::ImportModelFile(const std::filesystem::path& fileSysPath) {
    // Create an instance of the Importer class
    Assimp::Importer importer;
   
    try {
        if(!fileSysPath.has_extension())
            throw("[" + fileSysPath.string() + "] cannot deduce file type. Tossing.\n");

        if(fileSysPath.extension() == ".fbx") {
            throw("[" + fileSysPath.string() + "] uses an unsupported format. Consider converting to GLTF/GLB. Tossing.\n");
        }

    }  catch(const std::string& msg) {
        std::cout << msg;
        return false;
    }

    const aiScene* scene = importer.ReadFile(fileSysPath.string(), cPostProcFlags);

    // If the import failed, report it
    if (nullptr == scene) {
        std::cout << "ImportModelFile() failed: " << (importer.GetErrorString()) << "\n";
        return false;
    }

    processScene(scene, fileSysPath.filename().stem().string());

   // Everything will be cleaned up by the importer destructor
    return true;
}

// By using the getter for Models, hopefully the app can stop crashing if a model file wasn't located

std::shared_ptr<Model> CustomModelImporter::getModel(const std::string& modelName) {
    auto search = mImportedModels.find(modelName);
    if(search != mImportedModels.end())
        return search->second;
    else {
        std::cout << "[" << modelName << "] was not found.\n";
        return mImportedModels["unnamed_map" + std::to_string(mImportedModels.size())];
    }
}
