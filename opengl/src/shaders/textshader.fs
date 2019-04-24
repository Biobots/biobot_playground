#version 330 core
in vec3 outColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
	FragColor = mix(texture(ourTexture, TexCoord), vec4(outColor, 1.0f), 0.5);
	//FragColor = texture(ourTexture, TexCoord);
}