#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 light = normalize(lightPos - FragPos);
    float diff = max(dot(norm, light), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 objectColor = vec3(texture(ourTexture, TexCoord));
    vec3 result = (ambient + diffuse) * objectColor;
	FragColor = vec4(result, 1.0f);
}