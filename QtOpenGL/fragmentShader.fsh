#version 330 core

out vec4 fragmentColor;
in vec2 TexCoord;

uniform sampler2D texture0;

void main(void)
{
    fragmentColor = texture(texture0, TexCoord);
}
