#include "Texture.h"
#include <stb/stb_image.h>
#include <iostream>
#include "glad/glad.h"
#include <string>
#include <memory>
#include <filesystem>
#include <utility>
#include <vector>

static int sTextureSlot = 0;

const std::vector<std::string> Texture::scmFaceNames = {
	"right.png",
	"left.png",
	"top.png",
	"bottom.png",
	"front.png",
	"back.png"
};

constexpr int cDesiredColorChannels = 4; // All textures shall be outputted with RGB channels
using namespace std::filesystem;

Texture::Texture(const path& filePath, int target) : mSlotNum(0), mBitData(nullptr), mTextureId(0),
	mVertexIndex(0), mTextureTarget(target) {
	
	glGenTextures(1, &mTextureId);
	std::cout << "Texture ID of " << mTextureId << " made for target: " << ((target == GL_TEXTURE_2D) ? "2D" : "CubeMap") << "\n";
	glActiveTexture(GL_TEXTURE0 + sTextureSlot);
	mVertexIndex = sTextureSlot;
	mSlotNum = sTextureSlot;
	bind();

	// TwoDim
	if(target == GL_TEXTURE_2D) {
		auto [dat, w, h, ch] = loadImage(filePath.string().c_str(), cDesiredColorChannels);

		if(!dat) {
			std::cout << "Texture: " << filePath.string().c_str() << " failed to import\n";
			return;
		}
		std::cout << "Loaded " << filePath.string().c_str() << "\n";

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, dat.get());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else if(target == GL_TEXTURE_CUBE_MAP) {
		for(unsigned int i = 0; i < scmFaceNames.size(); i++) {
			auto p = filePath / scmFaceNames[i];
			auto [dat, w, h, ch] = loadImage(p.string().c_str(), cDesiredColorChannels);

			if(dat) {
				std::cout << "Loaded " << p.string() << "\n";
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, dat.get());
			} else
				std::cout << "Cubemap tex failed to load face: " << scmFaceNames[i] << " \n";
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	
	++sTextureSlot;
};

Texture::~Texture() {
	if(mTextureId != 0)
		glDeleteTextures(1, &mTextureId);
}

void Texture::bind() const {
	glBindTexture(mTextureTarget, mTextureId);
}

int Texture::getAssignedTextureSlot() const {
	return mSlotNum;
}

ReturnedImageData Texture::loadImage(const char* path, int desiredChannels) {
	int w{}, h{}, ch{};
	auto* data = stbi_load(path, &w, &h, &ch, desiredChannels);
	
	return ReturnedImageData( data, w, h, ch );
}