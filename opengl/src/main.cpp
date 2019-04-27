#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include "shader.h"
#include "test.h"

glm::mat4 view, projection;
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float deltaTime = 0.0f;
float preFrame = 0.0f;

float yaw, pitch;
float fov = 45.0f;

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
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
void mouseInput(GLFWwindow* window, double xPos, double yPos)
{
    static float xLast = 400;
    static float yLast = 300;
    static bool firstRun = true;
    //printf("%d, xLast:%f, yLast:%f. xPos:%f, yPos:%f", firstRun, xLast, yLast, xPos, yPos);;
    if(firstRun)
    {
        xLast = xPos;
        yLast = yPos;
        firstRun = false;
    }
    //printf("%d, xLast:%f, yLast:%f. xPos:%f, yPos:%f", firstRun, xLast, yLast, xPos, yPos);;

    float dx = xPos - xLast;
    float dy = yLast - yPos; //reverse
    xLast = xPos;
    yLast = yPos;

    float sensitivity = 0.05f;
    dx *= sensitivity;
    dy *= sensitivity;

    yaw += dx;
    pitch += dy;
    pitch = (pitch > 89.0f ? 89.0f : pitch);
    pitch = (pitch < -89.0f ? -89.0f : pitch);
    /*yaw = (yaw > 89.0f ? 89.0f : yaw);
    yaw = (yaw < -89.0f ? -89.0f : yaw);*/

    glm::vec3 front;
    
    //when initial cameraFront matrix is (0, 0, -1)
    front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = -1 * cos(glm::radians(pitch)) * cos(glm::radians(yaw));

    //when initial cameraFront matrix is (1, 0, 0)
    //front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    //front.y = sin(glm::radians(pitch));
    //front.z = -1 * cos(glm::radians(pitch)) * cos(glm::radians(yaw));


    cameraFront = glm::normalize(front);
}

void scrollInput(GLFWwindow* window, double dx, double dy)
{
    if (fov >= 1.0f && fov <= 45.0f)
    { fov -= dy;}
    if (fov <= 1.0f){fov = 1.0f;}
    if (fov >= 45.0f){fov = 45.0f;}

    projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
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

    test3Initialize();
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 0.5f, 0.5f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        test3Loop();
        
        // glfw: swap buffers and poll IO events (keyspressed/released, mouse moved etc.)
        // ---------------------------------------------------
        float curFrame = glfwGetTime();
        deltaTime = curFrame - preFrame;
        preFrame = curFrame;
        processInput(window);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        printf("front:%f, %f, %f\n", cameraFront.x, cameraFront.y, cameraFront.z);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFWresources.
    //---------------------------------------------------------------
    glfwTerminate();
    return 0;
}