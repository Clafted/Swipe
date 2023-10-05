#version 330 core

in vec2 textureCoord;
out vec4 FragColor;

uniform sampler2D txtr;
uniform vec4 wireframeColor = vec4(0.0, 0.0, 0.0, 0.0);

void main()
{
	FragColor = texture(txtr, textureCoord) + wireframeColor;
}