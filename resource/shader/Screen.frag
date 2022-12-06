#version 330 core

// Framebuffer texture
in vec2 TexCoord;
uniform sampler2D screenTexture;
uniform sampler2D noiseTexture;

// Night vision post-processing
uniform bool nightVision = false;
uniform float elapsedTime = 0.0;

vec3 ProcessNightVision(vec3 color, vec3 noise);

void main()
{
    vec2 uv; // swriling uv
    uv.x = 0.35 * sin(elapsedTime * 50.0);
    uv.y = 0.35 * cos(elapsedTime * 50.0); 
    // vec3 noise = vec3(0.0, 0.0, 0.0);
    vec3 noise = texture(noiseTexture, TexCoord.st + uv).rgb;

    vec3 color;
    if (nightVision)
        color = ProcessNightVision(texture(screenTexture, TexCoord.st + (noise.xy * 0.005)).rgb, noise);
    else
        color = texture(screenTexture, TexCoord).rgb;

    gl_FragColor = vec4(color, 1.0);
}

vec3 ProcessNightVision(vec3 color, vec3 noise)
{
    const vec3 rgbWeight = vec3(0.30, 0.59, 0.11);
    const float contrast = 0.5;

    float intensity = dot(rgbWeight, color);
    intensity = clamp(contrast * (intensity - 0.5) + 0.5, 0.0, 1.0);
    float green = clamp(intensity / rgbWeight.y, 0.0, 1.0);

    vec3 visionColor = vec3(0, green, 0);

    return (color + noise * 0.2) * visionColor;
}