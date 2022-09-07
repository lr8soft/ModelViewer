#version 330 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    //sampler2D shadow_map;
    float shininess;
};

uniform Material material;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{   
    FragColor = texture2D(material.texture_diffuse1, TexCoords);
}