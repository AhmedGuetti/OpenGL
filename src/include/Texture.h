#pragma once

#include "Renderer.h"

class Texture {
private:
	unsigned int m_RendererID;
	const std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; } // Added const
	inline int GetHeight() const { return m_Height; } // Added const


};