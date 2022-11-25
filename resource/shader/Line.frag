#version 330 core
in vec3 color;

// out vec4 fragColor;

void main()
{
    gl_FragColor = vec4(color, 1.0);
}