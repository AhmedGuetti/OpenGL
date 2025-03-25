#include "Renderer.h"
#include <iostream>

/**
 * Clear Error before the function
 *
 */
void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

/**
 * Get and print the errors in a function
 *
 * This function get show the linem function and the file with the error
 * it does work with older version of Gl < 4.6
 *
 * @param function, function name that have the error
 * @param file, file name where the error occured
 * @param line, The line the function is called and the error have benn found
 *
 * @return sum of `values`, or 0.0 if `values` is empty.
 */
bool GLLogError(const char* function, const char* file, int line) {
    bool hasError = false;
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")"
            << function << " " << file << " : " << line << std::endl;
        hasError = true;
    }
    return !hasError;
}


void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

// Added this method to set the clear color
void Renderer::SetClearColor(float r, float g, float b, float a) const {
    GLCall(glClearColor(r, g, b, a));
}

void Renderer::Clear() const {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}