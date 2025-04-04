#pragma once

#include <glad/glad.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogError(#x, __FILE__, __LINE__))



void GLClearError();
bool GLLogError(const char* function, const char* file, int line);



class Renderer {
public:
    void Clear() const;
    void SetClearColor(float r, float g, float b, float a) const; // Added this method
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;


};