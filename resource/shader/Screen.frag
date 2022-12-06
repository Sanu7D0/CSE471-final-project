#version 330 core

// Framebuffer texture
in vec2 TexCoord;
uniform sampler2D screenTexture;

// Night vision post-processing
uniform bool nightVision = false;
float contrast = 0.5;

void main()
{
    vec3 sceneColor = texture(screenTexture, TexCoord).rgb * 1.0;
    
    // if (nightVision)
    //     vec3 result = ProcessNightVision(sceneColor);

    gl_FragColor = vec4(sceneColor, 1.0);
}

vec3 ProcessNightVision(vec3 color)
{
    const vec3 rgbWeight = vec3(0.30, 0.59, 0.11);
    float intensity = dot(rgbWeight, color);

    intensity = clamp(contrast * (intensity - 0.5) + 0.5, 0.0, 1.0);
    return color;
}