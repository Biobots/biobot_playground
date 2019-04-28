#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float FOV        =  45.0f;

class Camera
{
private:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float fov;
    float speed;
    float sensitivity;

    void updateVectors()
    {
        glm::vec3 front;
        //TODO
        front.x = -1 * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = -1 * sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraFront = glm::normalize(front);
        cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
        cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));
        printf("front:%f, %f, %f\n", cameraFront.x, cameraFront.y, cameraFront.z);
    }
    

public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yawInput = YAW, float pitchInput = PITCH) : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), sensitivity(SENSITIVITY), fov(FOV)
    {
        cameraPos = position;
        worldUp = up;
        yaw = yawInput;
        pitch = pitchInput;
        updateVectors();
        this->position = position;
    }
    ~Camera()
    {
        
    }

    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 position;

    void updateMatrix()
    {
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
    }
    
    void mouseInput(double xPos, double yPos)
    {
        static float xLast = (float)SCR_WIDTH / 2;
        static float yLast = (float)SCR_HEIGHT / 2;
        static bool firstRun = true;

        if(firstRun)
        {
            xLast = xPos;
            yLast = yPos;
            firstRun = false;
        }

        float dx = xPos - xLast;
        float dy = yLast - yPos; //reverse
        xLast = xPos;
        yLast = yPos;

        dx *= sensitivity;
        dy *= sensitivity;

        yaw += dx;
        pitch += dy;
        pitch = (pitch > 89.0f ? 89.0f : pitch);
        pitch = (pitch < -89.0f ? -89.0f : pitch);

        updateVectors();
    }

    void scrollInput(double dx, double dy)
    {
        if (fov >= 1.0f && fov <= 45.0f)
        { fov -= dy;}
        if (fov <= 1.0f){fov = 1.0f;}
        if (fov >= 45.0f){fov = 45.0f;}
    }

    void keyboardInput(GLFWwindow *window, float deltaTime)
    {
        float speed = 2.5f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += speed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= speed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
        position = cameraPos;
    }

    glm::mat4 getView()
    {
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        return view;
    }

    glm::mat4 getProjection()
    {
        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
        return projection;
    }
};


#endif // !CAMERA_H