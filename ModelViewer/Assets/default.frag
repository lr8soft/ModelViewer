#version 330 core
uniform sampler2D texture_diffuse1;
in vec2 TexCoords;
out vec4 FragColor;

void main()
{   
    FragColor = texture2D(texture_diffuse1, TexCoords);
}