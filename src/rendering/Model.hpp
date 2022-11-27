#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "Mesh.hpp"
#include "Shader.hpp"

class Model
{
public:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	Model(std::string const &path, bool gamma = false) : gammaCorrection(gamma)
	{
		loadModel(path);
	}

	void draw(const Shader& shader) const
	{
		for (const auto& mesh : meshes)
			mesh.draw(shader);
	}

private:
	void loadModel(std::string const &path)
	{
		std::cout << "Load model: " << path << "\n";
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
		{
			std::cerr << "assimp: " << importer.GetErrorString() << std::endl;
			return;
		}

		// Retrieve the file directory
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	// Process a node recursively
	void processNode(const aiNode *node, const aiScene *scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (uint32_t i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(const aiMesh* mesh, const aiScene* scene)
	{
		// Process all vertices of the mesh
		std::vector<Vertex> vertices;
		for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
		{
			auto position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			auto normal = (mesh->HasNormals()
				               ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
				               : glm::vec3(0.0f, 0.0f, 0.0f));
			// Check the mesh contains texture coordinates
			glm::vec2 texCoord;
			if (mesh->mTextureCoords[0])
			{
				texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

				/*auto tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
				auto bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);*/
			}
			else
			{
				texCoord = glm::vec2(0.0f, 0.0f);
			}

			vertices.emplace_back(position, normal, texCoord);
		}

		// Retrieve the corresponding vertex indices
		std::vector<uint32_t> indices;
		for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
		{
			const aiFace& face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}

		// Process materials
		const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> textures;

		// diffuse: texture_diffuseN
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// specular: texture_specularN
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		// normal: texture_normalN
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		/*std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());*/

		return {vertices, indices, textures};
	}

	std::vector<Texture> loadMaterialTextures(const aiMaterial *material, aiTextureType type, const std::string &typeName)
	{
		std::cout << "Load material type: " << typeName << " " << material->GetTextureCount(type) << "\n";
		std::vector<Texture> textures;
		for (uint32_t i = 0; i < material->GetTextureCount(type); ++i)
		{
			aiString str;
			material->GetTexture(type, i, &str);

			bool skip = false; // Check if the texture loaded once before
			for (const auto& texture : textures_loaded)
			{
				if (std::strcmp(texture.path.data(), str.C_Str()) == 0)
				{
					textures.push_back(texture);
					skip = true; // The texture has already been loaded
					break;
				}
			}
			if (!skip)
			{
				Texture texture;
				texture.id = textureFromFile(str.C_Str(), directory, gammaCorrection);
				texture.type = typeName;
				texture.path = str.C_Str();

				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}
		return textures;
	}

	GLuint textureFromFile(const char* path, const std::string& directory_, bool gamma) const
	{
		std::string filename = std::string(path);
		filename = directory_ + '/' + filename; // windows: '\\'

		uint32_t textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		if (unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0))
		{
			GLenum format = GL_RGB;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
			return 0;
		}

		std::cout << "Load texture: " << path << std::endl;
		return textureID;
	}
};
