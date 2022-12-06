#version 330 core

in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{
    vec3 color = texture(screenTexture, TexCoord).rgb;
    gl_FragColor = vec4(color, 1.0);
}