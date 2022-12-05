#pragma once
#include <glm/glm.hpp>

#include <vector>
#include <memory>

#include "Shader.hpp"

class Light
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    bool enabled = true;

    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
};

class DirLight : public Light
{
public:
    glm::vec3 direction;

    DirLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
};


class PointLight : public Light
{
public:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position,
        float constant, float linear, float quadratic);
};

class SpotLight : public Light
{
public:
    glm::vec3 position;
    glm::vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;

    SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction,
        float constant, float linear, float quadratic, float cutOff, float outerCutOff);
};

// Dynamic singleton class
class LightManager
{
private:
    LightManager() {}
    ~LightManager() {}

    // Not copyable
    LightManager(const LightManager&);
    LightManager& operator= (const LightManager&);

    static LightManager* _instance;

public:
    static LightManager* Instance()
    {
        if (_instance == nullptr) _instance = new LightManager();
        return _instance;
    }

    void setDirLight(DirLight light, const Shader& shader);

    std::shared_ptr<PointLight> addPointLight(PointLight light, const Shader& shader);

    std::shared_ptr<SpotLight> addSpotLight(SpotLight light, const Shader& shader);

    void update(const Shader& shader);

private:
    std::unique_ptr<DirLight> dirLight;
    std::vector<std::shared_ptr<PointLight>> pointLights;
    std::vector<std::shared_ptr<SpotLight>> spotLights;
};
