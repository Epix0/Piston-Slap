#pragma once
#include <iostream>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

class CustomIModelImporter
{
public:
	bool ImportModelFile(const std::string& pFile);

private:
	void ProcessScene(const aiScene* scene);
};