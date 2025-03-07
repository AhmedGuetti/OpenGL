#pragma once
#include <glad/glad.h>
#include <vector>
#include "Renderer.h"


struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    
    static unsigned int getSizeType(unsigned int type){
        switch (type)
        {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_BYTE: return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout()
        :m_Stride(0) {}
    
    ~VertexBufferLayout() = default;

    template<typename T>
    void Push(unsigned int count) {
        static_assert(false);
    }

    template<>
    void Push<float>(unsigned int count) {
        m_Elements.push_back({ GL_FLOAT, (unsigned int)count, GL_FALSE });
        m_Stride += count * VertexBufferElement::getSizeType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count) {
        m_Elements.push_back({ GL_UNSIGNED_INT, (unsigned int)count, GL_FALSE });
        m_Stride += count * VertexBufferElement::getSizeType(GL_UNSIGNED_INT);


    }

    template<>
    void Push<unsigned char>(unsigned int count) {
        m_Elements.push_back({ GL_UNSIGNED_BYTE, (unsigned int)count, GL_TRUE });
        m_Stride += count *  VertexBufferElement::getSizeType(GL_UNSIGNED_BYTE);

    }

    inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }

};
