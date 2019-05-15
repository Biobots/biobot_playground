#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 light = normalize(lightPos - FragPos);
    float diff = max(dot(norm, light), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-light, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //This 32 value is the shininess value of the highlight
    vec3 specular = specularStrength * spec * lightColor;

    vec3 objectColor = vec3(texture(ourTexture, TexCoord));
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0f);
}