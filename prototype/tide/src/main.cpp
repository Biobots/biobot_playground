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

const int COL = 200;  //y
const int ROW = 200; //x
glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, -1.0f, 1.0f);

Cell* cells[COL][ROW];

float xoffset = -0.5;
float yoffset = -0.5;
float scale = 2.5;
float pointSize = 1.25;

int totalsize = sizeof(cells) / sizeof(Cell*);
int rowsize = sizeof(cells[0]) / sizeof(Cell*);
int colsize = totalsize / rowsize;
float vertices[] = {
    pointSize, pointSize,
    pointSize, -pointSize,
    -pointSize, -pointSize,
    -pointSize,  pointSize
};
unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
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
void processCellLogic()
{
    for (int i = 1; i < colsize - 1; i++)
    {
        for (int j = 1; j < rowsize - 1; j++)
        {
            Cell* cell = cells[i][j];
            int num = (int)cells[i][j-1]->isExistCopy + (int)cells[i-1][j]->isExistCopy + 
                (int)cells[i][j+1]->isExistCopy + (int)cells[i+1][j]->isExistCopy +
                (int)cells[i-1][j-1]->isExistCopy + (int)cells[i-1][j+1]->isExistCopy + 
                (int)cells[i+1][j+1]->isExistCopy + (int)cells[i+1][j-1]->isExistCopy;
            if (num >= 3 && num <= 5)
            {
                cell->setExistence(true);
            }
            else
            {
                cell->setExistence(false);
            }
        }
    }
    
}
void mouseInput(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        printf("clicked1:%f, %f\n", (float)xPos, (float)yPos);
        for (int i = 0; i < colsize; i++)
        {
            for (int j = 0; j < rowsize; j++)
            {
                Cell* cell = cells[i][j];
                if (xPos >= cell->xPos - pointSize && xPos <= cell->xPos + pointSize &&
                    yPos >= cell->yPos - pointSize && yPos <= cell->yPos + pointSize)
                    {
                        printf("pre:%d\n", (int)cell->getExistence());
                        cell->setExistence(!cell->getExistence());
                        printf("clicked2:%f ,%f\n", cell->xPos, cell->yPos);
                        printf("aft:%d\n", (int)cell->getExistence());
                    }
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
    //glEnable(GL_PROGRAM_POINT_SIZE); 
    
    
    float xoffset = SCR_WIDTH / 2 - (float)rowsize * scale / 2;
    float yoffset = SCR_HEIGHT / 2 - (float)colsize * scale / 2;
    
    for (int i = 0; i < colsize; i++)
    {
        for (int j = 0; j < rowsize; j++)
        {
            cells[i][j] = new Cell();
            cells[i][j]->setExistence(((i + j) % 2 == 0) ? true : false);
            cells[i][j]->xPos = (float)j * scale + xoffset;
            cells[i][j]->yPos = (float)i * scale + yoffset;
        }
    }

    unsigned int VAO, VBO, EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Shader shader("shaders\\cell.vs", "shaders\\cell.fs");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        for (int i = 0; i < colsize; i++)
        {
            for (int j = 0; j < rowsize; j++)
            {
                Cell* cell = cells[i][j];
                cell->isExistCopy = cell->getExistence();
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(cells[i][j]->xPos, cells[i][j]->yPos, 0));
                shader.setMat4("projection", glm::value_ptr(projection));
                shader.setMat4("model", glm::value_ptr(model));
                shader.setFloat("inColor", (int)cells[i][j]->getExistence());
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }   

        
        // glfw: swap buffers and poll IO events (keyspressed/released, mouse moved etc.)
        // ---------------------------------------------------
        float curFrame = glfwGetTime();
        deltaTime += curFrame - preFrame;
        preFrame = curFrame;
        if (deltaTime >= (float)1/10)
        {
            printf("time:%f\n", deltaTime);
            deltaTime = 0;
            processCellLogic();
        }
        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFWresources.
    //---------------------------------------------------------------
    glfwTerminate();
    return 0;
}