#include "CustomIModelImporter.h"
#include <string>

void CustomModelImporter::ProcessScene(const aiScene* scene) {
    //std::cout << scene->mMeshes[0]->mvert << "\n";
    this->numOfVertices = scene->mMeshes[0]->mNumVertices;

    if (this->numOfVertices > 3000) {
        std::cout << "WARNING: numOfVertices is greater than init value of CustomModelImporter member\n";
    }

    int needle = 0;
    std::cout << this->numOfVertices << "\n";
    for (int i = 0; i < 1966; i++) {
        std::cout << i << "\n";
        auto p = scene->mMeshes[0]->mVertices[i];
        this->vertices[needle] = p.x;
        this->vertices[needle + 1] = p.y;
        this->vertices[needle + 2] = p.z;
        needle += 3;
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