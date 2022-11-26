#version 330 core

in vec2 TexCoord;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    // Lights
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 32 = shininess
    vec3 specular = specularStrength * spec * lightColor;

    vec4 lightResult = vec4(ambient + diffuse + specular, 1.0);

    gl_FragColor = lightResult * texture(texture_diffuse1, TexCoord);
}