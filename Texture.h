#pragma once
#include <string>

class Texture {
public:
	Texture(const char* filename, const std::string& modelName);

	~Texture();

	void bind() const;

	inline unsigned int getGLTextureId() const { return mTextureId; }

	int getAssignedTextureSlot() const;

	// GL_TEXTURE[#]
	int mSlotNum;

	// Which vertex index this texture belongs to. Used with gl_VertexID
	int mVertexIndex;
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