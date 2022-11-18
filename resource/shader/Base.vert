#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 viewProj;
// uniform mat3 normalMatrix;

void main()
{
    // color = aColor;
    color = vec3(1.0, 1.0, 1.0);
    texCoord = aTexCoord;

    gl_Position = viewProj * model * vec4(aPosition, 1.0f);
}