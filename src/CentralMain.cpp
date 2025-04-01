#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>

#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"

// Include our new Shape classes
#include "Shape.h"
#include "Cube.h"
#include "Sphere.h"
#include "BezierSurface.h"

// Global variables
Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
float lastX = 400, lastY = 300;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Current shape selection
enum ShapeType { CUBE, SPHERE, BEZIER_SURFACE };
ShapeType currentShape = CUBE;

// Callback for window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Process keyboard input
void processInput(GLFWwindow* window, std::vector<std::unique_ptr<Shape>>& shapes)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    // Toggle wireframe mode
    static bool wireframeKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !wireframeKeyPressed)
    {
        for (auto& shape : shapes)
        {
            shape->ToggleWireframe();
        }
        wireframeKeyPressed = true;
        std::cout << "Wireframe mode: " << (shapes[currentShape]->IsWireframe() ? "ON" : "OFF") << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    {
        wireframeKeyPressed = false;
    }

    // Switch shapes
    static bool key1Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !key1Pressed)
    {
        currentShape = CUBE;
        key1Pressed = true;
        std::cout << "Selected shape: Cube" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE)
        key1Pressed = false;

    static bool key2Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !key2Pressed)
    {
        currentShape = SPHERE;
        key2Pressed = true;
        std::cout << "Selected shape: Sphere" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE)
        key2Pressed = false;

    static bool key3Pressed = false;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !key3Pressed)
    {
        currentShape = BEZIER_SURFACE;
        key3Pressed = true;
        std::cout << "Selected shape: Bezier Surface" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE)
        key3Pressed = false;
}

// Mouse callback for camera rotation
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed as y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Scroll callback for camera zoom
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Shape Hierarchy Demo", NULL, NULL);
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Capture mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shape Hierarchy Demo" << std::endl;
    std::cout << "WASD      - Move camera" << std::endl;
    std::cout << "Mouse     - Look around" << std::endl;
    std::cout << "Space     - Move up" << std::endl;
    std::cout << "Ctrl      - Move down" << std::endl;
    std::cout << "F         - Toggle wireframe mode" << std::endl;
    std::cout << "1,2,3     - Select shape (Cube, Sphere, Bezier Surface)" << std::endl;

    // Enable depth testing
    GLCall(glEnable(GL_DEPTH_TEST));

    // Create shapes
    std::vector<std::unique_ptr<Shape>> shapes;

    // Add a cube
    auto cube = std::make_unique<Cube>(1.0f, 1.0f, 1.0f);
    cube->SetPosition(glm::vec3(-2.5f, 0.0f, 0.0f));
    shapes.push_back(std::move(cube));

    // Add a sphere
    auto sphere = std::make_unique<Sphere>(1.0f, 32, 16);
    sphere->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    shapes.push_back(std::move(sphere));

    // Add a Bezier surface
    auto bezierSurface = std::make_unique<BezierSurface>(20, 20);
    bezierSurface->SetPosition(glm::vec3(2.5f, 0.0f, 0.0f));
    shapes.push_back(std::move(bezierSurface));

    {
        // Create shaders
        Shader shader("res/shaders/Basic3D.shader");

        // Prepare light properties
        glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

        // Create renderer
        Renderer renderer;
        renderer.SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            // Calculate delta time
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // Process input
            processInput(window, shapes);

            // Clear frame
            renderer.Clear();
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            // Prepare view and projection matrices
            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), 800.0f / 600.0f, 0.1f, 100.0f);

            // Bind shader and set common uniforms
            shader.Bind();
            shader.SetUniform3f("u_LightPosition", lightPos.x, lightPos.y, lightPos.z);
            shader.SetUniform3f("u_LightColor", lightColor.x, lightColor.y, lightColor.z);
            shader.SetUniform3f("u_ViewPosition", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

            // Set different colors for each shape
            if (currentShape == CUBE) {
                shader.SetUniform4f("u_Color", 0.8f, 0.1f, 0.1f, 1.0f); // Red for cube
            }
            else if (currentShape == SPHERE) {
                shader.SetUniform4f("u_Color", 0.1f, 0.8f, 0.1f, 1.0f); // Green for sphere
            }
            else {
                shader.SetUniform4f("u_Color", 0.1f, 0.1f, 0.8f, 1.0f); // Blue for Bezier surface
            }

            // Draw the current shape
            shapes[currentShape]->Draw(shader, view, projection);

            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}