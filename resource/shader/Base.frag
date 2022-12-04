#version 330 core

struct DirLight {
    bool enabled;

    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    bool enabled;

    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    bool enabled;

    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// textures
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

// lights
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[16];
uniform SpotLight spotLights[16];
uniform int pointLightCount = 0;
uniform int spotLightCount = 0;
uniform vec3 defaultAmbient = vec3(0.0, 0.0, 0.0);

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() 
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Lights
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    for (int i = 0; i < pointLightCount; i++)
    {
        if (!pointLights[i].enabled) continue;
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    for (int i = 0; i < spotLightCount; i++)
    {
        if (!spotLights[i].enabled) continue;
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
    }
    result += defaultAmbient * vec3(texture(texture_diffuse1, TexCoord));
    
    gl_FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16); // 16 -> material.shiness

    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoord));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float lightDistance = length(light.position - fragPos);
    float attenuation = 1.0 / 
        (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16); // 16 -> material.shiness

    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoord));

    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float lightDistance = length(light.position - fragPos);
    float attenuation = 1.0 / 
        (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16); // 16 -> material.shiness

    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoord));

    return (ambient + diffuse + specular) * attenuation * intensity;
}
