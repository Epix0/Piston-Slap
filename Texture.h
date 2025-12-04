#pragma once
#include <string>
#include <memory>
#include <stb/stb_image.h>
#include <filesystem>
#include <glad/glad.h>
#include <vector>

using namespace std::filesystem;

struct ReturnedImageData {
	std::unique_ptr<stbi_uc> mData;
	int loadedWidth;
	int loadedHeight;
	int loadedChannelCount;

	ReturnedImageData(stbi_uc*& pDat, int w, int h, int ch):
		mData(pDat), loadedHeight(h), loadedWidth(w), loadedChannelCount(ch)
	{};

	~ReturnedImageData() = default;
};

class Texture {
public:
	enum class TextureTarget: unsigned int {
		TwoDim = GL_TEXTURE_2D,
		Cubemap = GL_TEXTURE_CUBE_MAP,
	};

	/// <summary>
	/// If @target is TwoDim, @filePath should lead straight
	/// to the image file.
	/// If @target is Cubemap, @filePath should lead to the dir of the faces
	/// </summary>
	/// <param name="filePath">
	/// Path relative to the exe
	/// </param>
	/// <param name="target">
	///	Modifies how the texture will be loaded
	/// </param>
	Texture(const path& filePath, int target);

	~Texture();

	void bind() const;

	inline unsigned int getGLTextureId() const { return mTextureId; }

	int getAssignedTextureSlot() const;

	// GL_TEXTURE[#]
	int mSlotNum;

	// Which vertex index this texture belongs to. Used with gl_VertexID
	int mVertexIndex;
private:
	ReturnedImageData loadImage(const char* path, int desiredChannels=0);

	// The return ptr from stb_iamge
	std::unique_ptr<stbi_uc> mBitData;

	GLenum mTextureTarget;

	// GL handle
	unsigned int mTextureId;

	// Reserved cubemap texture names per file
	static const std::vector<std::string> scmFaceNames;
#if false
	// Loaded texture names
	typedef std::string TextureName;
	static std::unordered_map<TextureName, Texture*> mLoadedTexturers;
#endif
};