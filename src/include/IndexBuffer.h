#pragma once


class IndexBuffer
{
private:
	unsigned int m_Render_ID;
	unsigned int m_Count;
public:
	IndexBuffer();
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const  { return m_Count; }
};
