#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include "shader.h"
#include "camera.h"
#include "cell.h"
#include "gameobject.h"

float deltaTime = 0.0f;
float preFrame = 0.0f;

const int COL = 10;  //y
const int ROW = 10; //x
glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, -1.0f, 1.0f);

Cell* cells[COL][ROW];

float xoffset = -0.5;
float yoffset = -0.5;
float scale = 50;
float pointSize = 20;

int totalsize = sizeof(cells) / sizeof(Cell*);
int rowsize = sizeof(cells[0]) / sizeof(Cell*);
int colsize = totalsize / rowsize;
float vertices[] = {
    0.0f, 0.0f
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
}
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
void mouseInput(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        printf("clicked1");
        float xCursor = ((float)xPos - xoffset * SCR_WIDTH);
        float yCursor = SCR_HEIGHT - ((float)yPos + yoffset * SCR_HEIGHT);
        printf("x:%f y:%f\n", xCursor, yCursor);
        for (int i = 0; i < colsize; i++)
        {
            for (int j = 0; j < rowsize; j++)
            {
                Cell cell = *cells[i][j];
                printf("x:%f y:%f\n", SCR_WIDTH*cell.xPos, SCR_HEIGHT*cell.yPos);
            }
        }
    }
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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,"Prototype", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(window, mouseInput);
    //glfwSetScrollCallback(window, scrollInput);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_PROGRAM_POINT_SIZE); 
    
    
    
    
    for (int i = 0; i < colsize; i++)
    {
        for (int j = 0; j < rowsize; j++)
        {
            cells[i][j] = new Cell();
            cells[i][j]->setExistence(((i + j) % 2 == 0) ? true : false);
            cells[i][j]->xPos = (float)j * scale;
            cells[i][j]->yPos = (float)i * scale;
        }
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    Shader shader("shaders\\cell.vs", "shaders\\cell.fs");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        shader.setFloat("pointSize", 10.0f);
        for (int i = 0; i < colsize; i++)
        {
            for (int j = 0; j < rowsize; j++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(cells[i][j]->xPos, cells[i][j]->yPos, 0));
                shader.setMat4("projection", glm::value_ptr(projection));
                shader.setMat4("model", glm::value_ptr(model));
                shader.setFloat("inColor", (int)cells[i][j]->getExistence());
                glDrawArrays(GL_POINTS, 0, 1);
            }
        }   

        
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