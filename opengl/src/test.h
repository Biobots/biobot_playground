#ifndef TEST_H
#define TEST_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"


extern glm::mat4 view;
extern glm::mat4 projection;
extern Camera mainCamera;

void test1Initialize();
void test1Loop();

void test2Initialize();
void test2Loop();

void test3Initialize();
void test3Loop();

void test4Initialize();
void test4Loop();

void test5Initialize();
void test5Loop();

void test6Initialize();
void test6Loop();

#endif