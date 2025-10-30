#include "CustomIModelImporter.h"
#include <string>

void CustomModelImporter::ProcessScene(const aiScene* scene) {
    //std::cout << scene->mMeshes[0]->mvert << "\n";
    aiMesh* pMainMesh = scene->mMeshes[0];
    this->numOfVertices = pMainMesh->mNumVertices;
    this->numOfFaces = pMainMesh->mNumFaces;

    std::cout << "Copying faces list...\n";
    for (unsigned int i = 0; i < pMainMesh->mNumFaces; i++) {
        for (unsigned int j = 0; j < 3; j++) {
            this->elementList.push_back(pMainMesh->mFaces[i].mIndices[j]);
        }
    }

    if (this->numOfVertices > 6000) {
        std::cout << "WARNING: numOfVertices is greater than init value of CustomModelImporter member\n";
    }

    this->vertices.reserve(pMainMesh->mNumVertices);
    for (unsigned int i = 0; i < this->numOfVertices; i++) {
        auto p = pMainMesh->mVertices[i];
        this->vertices.insert(this->vertices.end(), { p.x, p.y, p.z });
    }
    std::cout << "All vertices cached!\n";
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