#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <string>
#include <memory>

#include "Shader.hpp"

class Light
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    bool bEnabled = true;

    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : ambient(ambient), diffuse(diffuse), specular(specular)
    {}

    void enable() { bEnabled = true; }
    void disable() { bEnabled = false; }
};

class DirLight : public Light
{
public:
    glm::vec3 direction;

    DirLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction)
        : Light(ambient, diffuse, specular), direction(direction)
    {}
};

class PointLight : public Light
{
public:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position,
        float constant, float linear, float quadratic)
        : Light(ambient, diffuse, specular), position(position),
        constant(constant), linear(linear), quadratic(quadratic)
    {}
    
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

    SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position,
        float constant, float linear, float quadratic, float cutOff, float outerCutOff)
        : Light(ambient, diffuse, specular), position(position),
        constant(constant), linear(linear), quadratic(quadratic),
        cutOff(cutOff), outerCutOff(outerCutOff)
    {}
};

class LightManager
{
public:
    LightManager(DirLight dirLight, const Shader& shader)
        : dirLight(dirLight)
    {
        shader.use();
        shader.setVec3("dirLight.direction", dirLight.direction);
        shader.setVec3("dirLight.ambient", dirLight.ambient);
        shader.setVec3("dirLight.diffuse", dirLight.diffuse);
        shader.setVec3("dirLight.specular", dirLight.specular);
    }

    std::shared_ptr<PointLight> addPointLight(PointLight light, const Shader& shader)
    {
        const auto idx = std::to_string(pointLights.size());

        shader.use();
        shader.setInt("pointLightCount", pointLights.size() + 1);
        shader.setBool("pointLights[" + idx + "].enabled", light.bEnabled);
        shader.setVec3("pointLights[" + idx + "].position", light.position);
        shader.setVec3("pointLights[" + idx + "].ambient", light.ambient);
        shader.setVec3("pointLights[" + idx + "].diffuse", light.diffuse);
        shader.setVec3("pointLights[" + idx + "].specular", light.specular);
        shader.setFloat("pointLights[" + idx + "].constant", light.constant);
        shader.setFloat("pointLights[" + idx + "].linear", light.linear);
        shader.setFloat("pointLights[" + idx + "].quadratic", light.quadratic);

        auto ptr = std::make_shared<PointLight>(light);
        pointLights.push_back(ptr);
        return ptr;
    }

    std::shared_ptr<SpotLight> addSpotLight(SpotLight light, const Shader& shader)
    {
    	const auto idx = std::to_string(spotLights.size());

        shader.use();
        shader.setInt("spotLightCount", spotLights.size() + 1);
        shader.setBool("spotLights[" + idx + "].enabled", light.bEnabled);
        shader.setVec3("spotLights[" + idx + "].position", light.position);
        shader.setVec3("spotLights[" + idx + "].direction", light.direction);
        shader.setVec3("spotLights[" + idx + "].ambient", light.ambient);
        shader.setVec3("spotLights[" + idx + "].diffuse", light.diffuse);
        shader.setVec3("spotLights[" + idx + "].specular", light.specular);
        shader.setFloat("spotLights[" + idx + "].constant", light.constant);
        shader.setFloat("spotLights[" + idx + "].linear", light.linear);
        shader.setFloat("spotLights[" + idx + "].quadratic", light.quadratic);
        shader.setFloat("spotLights[" + idx + "].cutOff", light.cutOff);
        shader.setFloat("spotLights[" + idx + "].outerCutOff", light.outerCutOff);

        auto ptr = std::make_shared<SpotLight>(light);
        spotLights.push_back(ptr);
        return ptr;
    }

    void update(const Shader& shader)
    {
        shader.use();
        for (int i = 0; i < pointLights.size(); ++i)
        {
            shader.setBool("pointLights[" + std::to_string(i) + "].enabled", pointLights[i]->bEnabled);
            shader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLights[i]->position);
        }

        for (int i = 0; i < spotLights.size(); ++i)
        {
            shader.setBool("pointLights[" + std::to_string(i) + "].enabled", spotLights[i]->bEnabled);
            shader.setVec3("spotLights[" + std::to_string(i) + "].position", spotLights[i]->position);
            shader.setVec3("spotLights[" + std::to_string(i) + "].direction", spotLights[i]->direction);
        }
    }

private:
    DirLight dirLight;
    std::vector<std::shared_ptr<PointLight>> pointLights;
    std::vector<std::shared_ptr<SpotLight>> spotLights;
};