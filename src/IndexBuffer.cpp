#include "IndexBuffer.h"

#include "Renderer.h"


IndexBuffer::IndexBuffer() {
	GLCall(glGenBuffers(1, &m_Render_ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Render_ID));

}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	:m_Count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint)); 
	GLCall(glGenBuffers(1, &m_Render_ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Render_ID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int  ), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_Render_ID));
}


void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Render_ID));

}
void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

}