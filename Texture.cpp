#include "Texture.h"
#include <stb/stb_image.h>
#include <iostream>
#include "glad/glad.h"
#include <string>
#include <memory>

static int sTextureSlot = 0;
constexpr int cDesiredColorChannels = 4; // All textures shall be outputted with RGB channels

Texture::Texture(const char* filename, const std::string& modelName) :
	mSlotNum(0), mBitData(nullptr), mTextureId(0), mVertexIndex(0)	{
	int width = 0;
	int height = 0;
	int channels = 0;
	std::string finalPath = std::string("textures/") + modelName + "/" + std::string(filename);
	
	// the return of stbi isn't used beyond tex loading, so a local is used for now instead of mBitData
	auto bitData = std::make_unique<stbi_uc>(stbi_load(finalPath.c_str(), &width, &height, &channels, cDesiredColorChannels));

	if (!bitData)
	{
		std::cout << "Texture: " << finalPath << " failed to import\n";
		return;
	}

	std::cout << "Loaded " << filename << "\n";

	glGenTextures(1, &mTextureId);
	glActiveTexture(GL_TEXTURE0 + sTextureSlot);
	mSlotNum = sTextureSlot;
	mVertexIndex = sTextureSlot;
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitData.get());
	glGenerateMipmap(GL_TEXTURE_2D);
	
	++sTextureSlot;
};

Texture::~Texture() {
	if (mTextureId != 0)
		glDeleteTextures(1, &mTextureId);
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, mTextureId);
}

int Texture::getAssignedTextureSlot() const {
	return mSlotNum;
}
