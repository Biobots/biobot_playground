#include "gameobject.h"

float GameObject::getXpos()
{
	return xpos;
}

float GameObject::getYpos()
{
	return ypos;
}

void GameObject::setXpos(float value)
{
	xpos = value;
}

void GameObject::setYpos(float value)
{
	ypos = value;
}

int GameObject::render()
{
    vector<GameObject>::iterator iter = Children.begin();
    for (; iter != Children.end(); iter++)
    {
        iter->render();
    }
    return 0;
}