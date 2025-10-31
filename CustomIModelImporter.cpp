#include "CustomIModelImporter.h"
#include <string>

void CustomModelImporter::ProcessScene(const aiScene* scene) {
    //std::cout << scene->mMeshes[0]->mvert << "\n";
    aiMesh* pMainMesh = scene->mMeshes[0];
    this->numOfVertices = pMainMesh->mNumVertices;
    this->numOfFaces = pMainMesh->mNumFaces;

    this->elementList.reserve(pMainMesh->mNumFaces);
    this->vertices.reserve(pMainMesh->mNumVertices);
    this->normals.reserve(pMainMesh->mNumVertices);
    this->normals.reserve((pMainMesh->mNumVertices * (static_cast<int64_t>(2)))); //stfu linter

    for (unsigned int i = 0; i < pMainMesh->mNumFaces; i++) {
        for (unsigned int j = 0; j < 3; j++) {
            this->elementList.push_back(pMainMesh->mFaces[i].mIndices[j]);
        }
    }
    
    // Vertices are stored as such:
    // { A_posX, A_posY, A_posZ, A_normalX, A_normalY, A_normalZ
    // B_posX, B_posY, B_posZ, B_normalX, B_normalY, B_normalZ ... }

    // Anything bearing the same range as mNumVertices...
    for (unsigned int i = 0; i < this->numOfVertices; i++) {
        // handling vertices
        auto pVertex = pMainMesh->mVertices[i];
        this->vertices.insert(this->vertices.end(), { pVertex.x, pVertex.y, pVertex.z });

        // handling normals
        auto pNormal = pMainMesh->mNormals[i];
        this->normals.insert(this->normals.end(), { pNormal.x, pNormal.y, pNormal.z });

        this->sortedBuffer.insert(this->sortedBuffer.end(), { pVertex.x, pVertex.y, pVertex.z, pNormal.x, pNormal.y, pNormal.z });
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