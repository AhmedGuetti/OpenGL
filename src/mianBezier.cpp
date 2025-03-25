#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "BezierCurve.h"

// Callback for window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Process keyboard input
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Control points for the Bezier curve
BezierCurve bezierCurve(100);
bool addPointMode = false;

// Mouse callback for adding control points
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && addPointMode)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Convert from screen coordinates to OpenGL coordinates (-1 to 1)
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        float x = (2.0f * xpos / width) - 1.0f;
        float y = 1.0f - (2.0f * ypos / height);

        // Add the point to our curve
        bezierCurve.AddControlPoint(glm::vec2(x, y));
        std::cout << "Added control point at (" << x << ", " << y << ")" << std::endl;
    }
}

// Key callback for toggling add point mode
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        addPointMode = !addPointMode;
        std::cout << (addPointMode ? "Add point mode ON" : "Add point mode OFF") << std::endl;
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        bezierCurve.ClearControlPoints();
        std::cout << "Control points cleared" << std::endl;
    }
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Bezier Curve Demo", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Press 'A' to toggle add point mode (currently " << (addPointMode ? "ON" : "OFF") << ")" << std::endl;
    std::cout << "Press 'C' to clear control points" << std::endl;
    std::cout << "Click to add control points when in add point mode" << std::endl;

    {
        // Add some initial control points for the Bezier curve
        bezierCurve.AddControlPoint(glm::vec2(-0.8f, -0.8f));
        bezierCurve.AddControlPoint(glm::vec2(-0.3f, 0.8f));
        bezierCurve.AddControlPoint(glm::vec2(0.3f, -0.8f));
        bezierCurve.AddControlPoint(glm::vec2(0.8f, 0.8f));

        // Create shader
        Shader shader("res/shaders/Bezier.shader");

        // Identity matrix for now (no transformation)
        glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = projection * view * model;

        // Create renderer
        Renderer renderer;
        renderer.SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        // Enable blending for transparency
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            // Input
            processInput(window);

            // Render
            renderer.Clear();

            const std::vector<float>& curvePoints = bezierCurve.GetCurvePoints();
            const std::vector<glm::vec2>& controlPoints = bezierCurve.GetControlPoints();

            if (!curvePoints.empty())
            {
                // Set up buffer for curve
                VertexArray vaCurve;
                VertexBuffer vbCurve(curvePoints.data(), curvePoints.size() * sizeof(float));
                VertexBufferLayout layoutCurve;

                layoutCurve.Push<float>(2);  // 2D positions
                vaCurve.AddBuffer(vbCurve, layoutCurve);

                // Draw curve
                shader.Bind();
                shader.SetUniform4f("u_Color", 1.0f, 0.5f, 0.0f, 1.0f);  // Orange

                // Add a SetUniformMat4f method in your shader class, or use this direct approach:
                GLCall(int mvpLocation = glGetUniformLocation(shader.GetRendererID(), "u_MVP"));
                if (mvpLocation != -1) {
                    GLCall(glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]));
                }

                vaCurve.Bind();
                GLCall(glDrawArrays(GL_LINE_STRIP, 0, bezierCurve.GetPointCount()));
            }

            // Draw control points and polygon if we have them
            if (!controlPoints.empty())
            {
                // Flatten the control points for the buffer
                std::vector<float> flatControlPoints;
                for (const auto& point : controlPoints)
                {
                    flatControlPoints.push_back(point.x);
                    flatControlPoints.push_back(point.y);
                }

                // Set up buffer for control points
                VertexArray vaControlPoints;
                VertexBuffer vbControlPoints(flatControlPoints.data(), flatControlPoints.size() * sizeof(float));
                VertexBufferLayout layoutControlPoints;
                layoutControlPoints.Push<float>(2);  // 2D positions
                vaControlPoints.AddBuffer(vbControlPoints, layoutControlPoints);

                // Draw control points
                shader.Bind();
                shader.SetUniform4f("u_Color", 0.2f, 0.8f, 0.2f, 1.0f);  // Green

                // Use the same direct approach as above
                GLCall(int mvpLocation = glGetUniformLocation(shader.GetRendererID(), "u_MVP"));
                if (mvpLocation != -1) {
                    // Access the matrix data directly without using value_ptr
                    GLCall(glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]));
                }

                vaControlPoints.Bind();
                GLCall(glPointSize(10.0f));
                GLCall(glDrawArrays(GL_POINTS, 0, controlPoints.size()));

                // Draw control polygon (lines connecting control points)
                shader.SetUniform4f("u_Color", 0.5f, 0.5f, 0.5f, 0.5f);  // Gray, semi-transparent
                GLCall(glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size()));
            }

            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}