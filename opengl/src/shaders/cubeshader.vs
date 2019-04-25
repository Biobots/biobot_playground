#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
    Normal = (mat3(transpose(inverse(model)))) * aNormal; // transform normal vector to world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    //multiply the vertex position attribute with the model matrix only (not the view and projection matrix) to transform it to world space coordinates
}