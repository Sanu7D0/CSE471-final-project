#version 330 core

in vec3 TexCoord;
uniform samplerCube skybox;

void main()
{
    gl_FragColor = texture(skybox, TexCoord);
}