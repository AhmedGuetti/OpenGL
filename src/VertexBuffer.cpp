#include "VertexBuffer.h"

#include "Renderer.h"


VertexBuffer::VertexBuffer() {
	GLCall(glGenBuffers(1, &m_Render_ID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Render_ID));
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_Render_ID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Render_ID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_Render_ID));
}


void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Render_ID));

}
void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

}