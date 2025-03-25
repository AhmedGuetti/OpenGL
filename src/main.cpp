#include <glad/glad.h>
#include<iostream>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

/*
Fixed:
    - Texture is now uncommented
    - Added clear color
    - Fixed shader path
*/


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main()
{
    // Hints for the glfw windows creation
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // init a windows variable 
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "OpenGL Again", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    /* Load glad to use the openGL specification */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Set the viewport */
    printf("GL %s\n", glGetString(GL_VERSION));
    // We add a scope here 
    {
        // Vertex Array
        float positions[] = {
            -0.5f,-0.5f, 0.0f, 0.0f,
             0.5f,-0.5f, 1.0f, 0.0f,
             0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f
        };

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // Index Buffer 
        unsigned int indices[] = {
           0,1,2,
           2,3,0
        };
        IndexBuffer ib(indices, 6);

        // Shader
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.4f, 0.256f, 0.6f, 1.0f);

        // Texture - Uncommented
        Texture texture("res/texture/chess.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        // Renderer
        Renderer renderer;
        // Set a clear color (not black)
        renderer.SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Debugging information
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        // Can't access private member GetUniformLocation

        // Cleaning
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Control Keys*/
            processInput(window);

            renderer.Clear();

            shader.Bind();
            texture.Bind(); // Make sure the texture is bound each frame
            shader.SetUniform4f("u_Color", r, 0.256f, 0.6f, 1.0f);
            renderer.Draw(va, ib, shader);

            // some Logic
            if (r > 1.0f) increment = -0.05f;
            else if (r < 0.0f) increment = 0.05f;

            r += increment;
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}