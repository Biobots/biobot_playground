#include "test.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

//test2
void test2Initialize()
{

}

void test2Loop()
{

}