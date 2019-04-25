#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

class Shader {
public:
	//����ID
	unsigned int ID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	//ʹ����ɫ��
	void use();
	//����uniform����
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, float value[]) const;
	void setVec4(const std::string& name, glm::vec4 value) const;
    void setVec3(const std::string& name, glm::vec3 value) const;
};

#endif