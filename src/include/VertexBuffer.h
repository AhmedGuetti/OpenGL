#pragma once


class VertexBuffer
{
private:
	unsigned int m_Render_ID;

public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
