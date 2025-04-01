#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Sphere.h"
#include "Ray.h"
#include "RayTracer.h"

// Global variables
Camera camera(glm::vec3(0.0f, 0.0f, 4.0f));
float lastX = 400, lastY = 300;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Ray tracer global
RayTracer* g_RayTracer = nullptr;

// Camera movement states
bool cameraActive = false;

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

    // Camera movement (only if camera is active)
    if (cameraActive) {
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
    }
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

    // Only rotate camera if right mouse button is pressed
    if (cameraActive) {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// Mouse button callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Right mouse button activates camera movement
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            cameraActive = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else if (action == GLFW_RELEASE) {
            cameraActive = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    // Left mouse button casts a ray
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !cameraActive)
    {
        // Get cursor position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Get window size
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Generate ray from camera through clicked pixel
        Ray ray = g_RayTracer->GenerateRay(xpos, ypos, width, height);
        g_RayTracer->SetCurrentRay(ray);

        std::cout << "Ray cast from: ("
            << ray.GetOrigin().x << ", "
            << ray.GetOrigin().y << ", "
            << ray.GetOrigin().z << ") in direction ("
            << ray.GetDirection().x << ", "
            << ray.GetDirection().y << ", "
            << ray.GetDirection().z << ")" << std::endl;
    }
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Ray-Sphere Intersection", NULL, NULL);
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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Ray-Sphere Intersection Demo" << std::endl;
    std::cout << "Left Click - Cast ray at mouse position" << std::endl;
    std::cout << "Right Click + Drag - Move camera" << std::endl;
    std::cout << "WASD      - Move camera (when right mouse button is held)" << std::endl;
    std::cout << "Mouse Wheel - Zoom in/out" << std::endl;

    // Enable depth testing
    GLCall(glEnable(GL_DEPTH_TEST));

    // Create a sphere
    std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>(1.0f, 32, 16);

    // Create ray tracer
    g_RayTracer = new RayTracer(camera);
    g_RayTracer->AddShape(sphere.get());

    {
        // Create shaders
        Shader shader("res/shaders/Basic3D.shader"); // Use your existing shader
        Shader rayShader("res/shaders/Ray.shader");   // Add this new shader

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
            processInput(window);

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
            shader.SetUniform4f("u_Color", 0.2f, 0.6f, 0.8f, 1.0f); // Blue color for sphere

            // Draw the sphere
            sphere->Draw(shader, view, projection);

            // Draw the ray (if any)
            g_RayTracer->RenderRay(rayShader, view, projection);

            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    // Clean up
    delete g_RayTracer;

    glfwTerminate();
    return 0;
}