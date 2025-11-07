#include "CustomIModelImporter.h"
#include <string>

void CustomModelImporter::ProcessScene(const aiScene* scene) {
    aiMesh* pMainMesh = scene->mMeshes[0];


    if (scene->mNumMeshes > 1) {
        std::cout << "cannot process Mesh due to more than one mesh instanec for " << scene->mName.C_Str() << "\n";
        return;
    }

    // Creation of the Decoded Mesh
    decodedMesh = std::make_unique<Mesh>("TheMesh");

    // Alloc area
    decodedMesh->vertices.reserve(pMainMesh->mNumVertices);
    decodedMesh->elementList.reserve(pMainMesh->mNumFaces * static_cast<int64_t>(3));
    decodedMesh->vertices.reserve(pMainMesh->mNumVertices);
    decodedMesh->normals.reserve(pMainMesh->mNumVertices);

    // "EBO" portion
    for (unsigned int i = 0; i < pMainMesh->mNumFaces; ++i) {
        aiFace& face = pMainMesh->mFaces[i];
        for (unsigned int j = 0; j < 3; j++) {
            decodedMesh->elementList.push_back(face.mIndices[j]);
        }
    }
    
    // Anything bearing the same range as mNumVertices...
    for (unsigned int i = 0; i < pMainMesh->mNumVertices; ++i) {
        // handling vertices
        auto vertex = pMainMesh->mVertices[i];
        decodedMesh->vertices.emplace_back(vertex.x, vertex.y, vertex.z);
        // handling normals
        auto vertexNormal = pMainMesh->mNormals[i];
        decodedMesh->normals.emplace_back(vertexNormal.x, vertexNormal.y, vertexNormal.z);
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
    ProcessScene(scene);

    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}

std::unique_ptr<Mesh>&& CustomModelImporter::getDecodedMesh()
{
    return std::move(decodedMesh);
}
