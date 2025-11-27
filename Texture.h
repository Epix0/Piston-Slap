#pragma once
#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <stb/stb_image.h>

class Texture {
public:
	Texture(const char* filename);

	~Texture();

	void bind() const;

	int getAssignedTextureSlot() const;

	// GL_TEXTURE[#]
	int mSlotNum;
private:
	// The return ptr from stb_iamge
	unsigned char* mBitData;
	// GL handle
	unsigned int mTextureId;
#if false
	// Loaded texture names
	typedef std::string TextureName;
	static std::unordered_map<TextureName, Texture*> mLoadedTexturers;
#endif
};