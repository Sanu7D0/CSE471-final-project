#version 330 core
in vec2 texCoord;

// out vec4 fragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main()
{
    gl_FragColor = texture(texture_diffuse1, texCoord);
    // fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}