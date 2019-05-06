#include "test.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//global setting
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//test 1
float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
};
unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};
float verticesT[] = {
    0.25f,   0.25f,   0.0f,   1.0f,   0.0f,   0.0f,   1.0f,   1.0f,
    0.25f,   -0.25f,  0.0f,   0.0f,   1.0f,   0.0f,   1.0f,   0.0f,
    -0.25f,  -0.25f,  0.0f,   0.0f,   0.0f,   1.0f,   0.0f,   0.0f,
    -0.25f,  0.25f,   0.0f,   1.0f,   1.0f,   0.0f,   0.0f,   1.0f 
};
unsigned int EBO, VBO, VAO, VBOT, VAOT;
int width, height, nrChannels;
unsigned int texture;
unsigned char *data;
Shader* shader;
Shader* textShader;

//VAO, VBO, EBO binding
void test1Initialize()
{
    static Shader tmp1("src\\shaders\\test.vs", "src\\shaders\\test.fs");
    static Shader tmp2("src\\shaders\\textshader.vs", "src\\shaders\\textshader.fs");
    shader = &tmp1;
    textShader = &tmp2;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0); 
    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        printf("texture load success");
    }
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &VAOT);
    glBindVertexArray(VAOT);
    glGenBuffers(1, &VBOT);
    glBindBuffer(GL_ARRAY_BUFFER, VBOT);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesT), verticesT, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void test1Loop()
{
    shader->use();
    float timeValue = glfwGetTime();
    float greenValue = sin(timeValue * 10) / 2.0f + 0.5f;
    shader->setVec4("UserColor", glm::vec4(1.0f, greenValue, 0.0f, 1.0f));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    textShader->use();
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAOT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

//test2 transform
glm::mat4 model;
Shader* tranShader;
unsigned transTexture, VAO2, VBO2, EBO2;
void test2Initialize()
{
    model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    static Shader transtmp("src\\shaders\\transformshader.vs", "src\\shaders\\textshader.fs");
    tranShader = &transtmp;

    glGenTextures(1, &transTexture);
    glBindTexture(GL_TEXTURE_2D, transTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0); 
    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        printf("texture load success");
    }

    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesT), verticesT, GL_STATIC_DRAW);
    glGenBuffers(1, &EBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void test2Loop()
{
    tranShader->use();
    tranShader->setMat4("model", glm::value_ptr(model));
    tranShader->setMat4("view", glm::value_ptr(view));
    tranShader->setMat4("projection", glm::value_ptr(projection));
    glBindVertexArray(VAO2);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


//test3 lighting
float cube[] = 
{
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
};
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};
Shader* cubeShader;
unsigned cubeTexture, VAO3, VBO3;
void test3Initialize()
{
    static Shader cubetmp("src\\shaders\\cubeshader.vs", "src\\shaders\\cubeshader.fs");
    cubeShader = &cubetmp;

    glGenTextures(1, &cubeTexture);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0); 
    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        printf("texture load success");
    }

    glGenVertexArrays(1, &VAO3);
    glBindVertexArray(VAO3);

    glGenBuffers(1, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

void test3Loop()
{
    cubeShader->use();
    //view = glm::mat4(1.0f);
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    //projection = glm::mat4(1.0f);
    //projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    mainCamera.updateMatrix();
    cubeShader->setMat4("view", glm::value_ptr(mainCamera.view));
    cubeShader->setMat4("projection", glm::value_ptr(mainCamera.projection));
    cubeShader->setVec3("viewPos", mainCamera.position);
    cubeShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    cubeShader->setVec3("lightPos", glm::vec3(0.0f, 0.0f, 5.0f));
    glBindVertexArray(VAO3);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * cos(i);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        cubeShader->setMat4("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

Shader* materialShader;
unsigned int VAO4, VBO4;
void test4Initialize()
{
    static Shader materialtmp("src\\shaders\\cubeshader.vs", "src\\shaders\\materialshader.fs");
    materialShader = &materialtmp;

    glGenTextures(1, &cubeTexture);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0); 
    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        printf("texture load success");
    }

    glGenVertexArrays(1, &VAO4);
    glBindVertexArray(VAO4);

    glGenBuffers(1, &VBO4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    printf("initialization success\n");
}

void test4Loop()
{
    materialShader->use();
    mainCamera.updateMatrix();
    materialShader->setMat4("view", glm::value_ptr(mainCamera.view));
    materialShader->setMat4("projection", glm::value_ptr(mainCamera.projection));
    materialShader->setVec3("viewPos", mainCamera.position);
    materialShader->setVec3("light.position", glm::vec3(0.0f, 0.0f, 5.0f));
    materialShader->setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    materialShader->setVec3("light.diffuse", glm::vec3(0.3f, 0.3f, 0.3f));
    materialShader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    materialShader->setVec3("material.ambient",  glm::vec3(0.3f, 0.5f, 0.7f));
    materialShader->setVec3("material.diffuse",  glm::vec3(0.3f, 0.5f, 0.7f));
    materialShader->setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    materialShader->setFloat("material.shininess", 128.0f);
    glBindVertexArray(VAO4);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * cos(i);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        materialShader->setMat4("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


Shader* lightmapShader;
unsigned int VAO5, VBO5;
unsigned int mapTexture;
void test5Initialize()
{
    static Shader lightmaptmp("src\\shaders\\cubeshader.vs", "src\\shaders\\lightmapshader.fs");
    lightmapShader = &lightmaptmp;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mapTexture);
    glBindTexture(GL_TEXTURE_2D, mapTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("container.png", &width, &height, &nrChannels, 0); 
    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //png use rgba!
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        printf("texture load success");
    }

    glGenVertexArrays(1, &VAO5);
    glBindVertexArray(VAO5);

    glGenBuffers(1, &VBO5);
    glBindBuffer(GL_ARRAY_BUFFER, VBO5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    printf("initialization success\n");
}

void test5Loop()
{
    lightmapShader->use();
    mainCamera.updateMatrix();
    lightmapShader->setMat4("view", glm::value_ptr(mainCamera.view));
    lightmapShader->setMat4("projection", glm::value_ptr(mainCamera.projection));
    lightmapShader->setVec3("viewPos", mainCamera.position);
    lightmapShader->setVec3("light.position", glm::vec3(0.0f, 0.0f, 5.0f));
    lightmapShader->setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    lightmapShader->setVec3("light.diffuse", glm::vec3(0.3f, 0.3f, 0.3f));
    lightmapShader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightmapShader->setInt("material.diffuse", 0);
    lightmapShader->setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightmapShader->setFloat("material.shininess", 128.0f);
    glBindVertexArray(VAO5);
    glBindTexture(GL_TEXTURE_2D, mapTexture);
    for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * cos(i);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        lightmapShader->setMat4("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}