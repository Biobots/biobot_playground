#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "interfaces.h"
#include <vector>

using namespace std;

class GameObject: public IRenderable
{
public:
	GameObject();
	~GameObject();

	int render();

	float getXpos();
	float getYpos();
	void setXpos(float value);
	void setYpos(float value);

    vector<GameObject> Children;

private:
	float xpos;
	float ypos;
};

#endif // !GAMEOBJECT_H
