#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include "shader.h"
#include "test.h"
#include "camera.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

glm::mat4 view, projection;
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float deltaTime = 0.0f;
float preFrame = 0.0f;

float yaw, pitch;
float fov = 45.0f;

Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));

void(*initFunc)() = test6Initialize;
void(*loopFunc)() = test6Loop;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    mainCamera.keyboardInput(window, deltaTime);
}
void mouseInput(GLFWwindow* window, double xPos, double yPos)
{
    mainCamera.mouseInput(xPos, yPos);
}

void scrollInput(GLFWwindow* window, double dx, double dy)
{
    mainCamera.scrollInput(dx, dy);
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //uncomment this statement to fix compilation on OS X
#endif
    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,"LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseInput);
    glfwSetScrollCallback(window, scrollInput);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST); 

    initFunc();
    Assimp::Importer importer;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 0.5f, 0.5f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        loopFunc();
        
        // glfw: swap buffers and poll IO events (keyspressed/released, mouse moved etc.)
        // ---------------------------------------------------
        float curFrame = glfwGetTime();
        deltaTime = curFrame - preFrame;
        preFrame = curFrame;
        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFWresources.
    //---------------------------------------------------------------
    glfwTerminate();
    return 0;
}