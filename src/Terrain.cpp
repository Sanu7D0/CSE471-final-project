#include "Terrain.hpp"

#include "rendering/Model.hpp"

Terrain::Terrain(Transform transform, Model model, BoxCollider collider)
	: GameObject(nullptr, transform, std::move(model)),
	collider(collider)
{
	collider.center = transform.position;
}

void Terrain::draw(const Shader& shader) const
{
	GameObject::draw(shader);

	if (Globals::debug)
		collider.draw(gAxesShader, transform.getModelMatrix());
}

TerrainManager::TerrainManager()
	//: skyboxShader(Shader("resource/shader/Skybox.vert", "resource/shader/Skybox.frag"))
{
	//loadTerrain();
	//loadCubemap();
}

void TerrainManager::init()
{
	loadTerrain();
}

void TerrainManager::draw(const Shader& shader) const
{
	for (const auto& t : terrains)
		t->draw(shader);
}

void TerrainManager::loadTerrain()
{
	terrains.push_back(std::make_shared<Terrain>( // make_shared -> new
		Transform(glm::vec3(5.0f, 3.0f, -5.0f)),  // ��ġ, ȸ��, ������,(����)
		Model("resource/model/magma_block.obj"),
		BoxCollider(glm::vec3(2.0f, 2.0f, 2.0f))   // ������ü heat_box (x,y,z)
		));
	/*terrains.push_back(std::make_shared<Terrain>(
		Transform(glm::vec3(0.0f, 0.0f, 3.0f)),
		Model("resource/model/magma_block.obj"),
		BoxCollider(glm::vec3(1.0f, 1.0f, 1.0f))// ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(glm::vec3(10.0f, -3.0f, 0.0f)),
		Model("resource/model/magma_block.obj"),
		BoxCollider(glm::vec3(1.0f, 1.0f, 1.0f))   // ������ü heat_box (x,y,z)
		));
		*/
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(10.0f, -0.9f, 0.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(90.0f), 0.0f)),
			glm::vec3(3.0f, 5.0f, 1.0f)),
		Model("resource/model/wall/wall.obj"),
		BoxCollider(glm::vec3(5.5f, 5.0f, 0.5f))   // ������ü heat_box (x,y,z)
		));
	//���� �� (����ó ��)
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(-10.0f, -0.9f, 0.0f), 
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(90.0f), 0.0f)),
			glm::vec3(3.0f, 5.0f, 1.0f)),
		Model("resource/model/wall/wall.obj"),
		BoxCollider(glm::vec3(5.5f, 5.0f, 0.5f))    // ������ü heat_box (x,y,z)
		));
	// ���� �� (����ó ��)
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(0.0f, -0.9f, 10.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), 0.0f, 0.0f)),
			glm::vec3(3.0f, 5.0f, 1.0f)),
		Model("resource/model/wall/wall.obj"),
		BoxCollider(glm::vec3(5.5f, 5.0f, 0.5f))    // ������ü heat_box (x,y,z)
		));
	//���� �� (����ó ��)
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(0.0f, -0.9f, -10.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), 0.0f, 0.0f)),
			glm::vec3(3.0f, 5.0f, 1.0f)),
		Model("resource/model/wall/wall.obj"),
		BoxCollider(glm::vec3(5.5f, 5.0f, 0.5f))    // ������ü heat_box (x,y,z)
		));
	// ���� �� (����ó ��)
	terrains.push_back(std::make_shared<Terrain>(
		Transform(glm::vec3(0.0f, -1.0f, 0.0f)),
		Model("resource/model/floor/floor2.obj"),
		BoxCollider(glm::vec3(100.0f, 0.01f, 100.0f))    // ������ü heat_box (x,y,z)
		));
	// �ٴ�
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(0.0f, 6.0f, 0.0f),                              //
			glm::quat(glm::vec3(glm::radians(180.0f), 0.0f, 0.0f)),
			glm::vec3(1.0f, 1.0f, 1.0f)
		),
		Model("resource/model/floor/floor2.obj"), // ceiling (upside down floor)
		BoxCollider(glm::vec3(100.0f, 0.01f, 100.0f))    // ������ü heat_box (x,y,z)
		));
	// õ��
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(0.0f, 3.0f, -50.0f),                              //
			glm::quat(glm::vec3(glm::radians(90.0f), glm::radians(0.0f), 0.0f)),
			glm::vec3(1.0f, 1.0f, 1.0f)
		),
		Model("resource/model/floor/floor2.obj"), // ceiling (upside down floor)
		BoxCollider(glm::vec3(100.0f, 0.5f, 8.0f))     // ������ü heat_box (x,y,z)
		));
	// ���� ��
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(0.0f, 3.0f, 50.0f),                              //
			glm::quat(glm::vec3(glm::radians(270.0f), 0.0f, 0.0f)),
			glm::vec3(1.0f, 1.0f, 1.0f)
		),
		Model("resource/model/floor/floor2.obj"), // ceiling (upside down floor)
		BoxCollider(glm::vec3(100.0f, 0.5f, 8.0f))    // ������ü heat_box (x,y,z)
		));
	//���� ��
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(50.0f, 3.0f, 0.0f),                              //
			glm::quat(glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(90.0f))),
			glm::vec3(1.0f, 1.0f, 1.0f)
		),
		Model("resource/model/floor/floor2.obj"), // ceiling (upside down floor)
		BoxCollider(glm::vec3(100.0f, 0.5f, 8.0f))    // ������ü heat_box (x,y,z)
		));
	//���� ��
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(-50.0f, 3.0f, 0.0f),                              //
			glm::quat(glm::vec3(glm::radians(0.0f), 0.0f, glm::radians(270.0f))),
			glm::vec3(1.0f, 1.0f, 1.0f)
		),
		Model("resource/model/floor/floor2.obj"), // ceiling (upside down floor)
		BoxCollider(glm::vec3(100.0f, 0.5f, 8.0f))   // ������ü heat_box (x,y,z)
		));
	//���� ��
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(-8.6f, -0.9f, -30.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(90.0f), 0.0f)),
			glm::vec3(6.99f, 5.0f, 1.0f)),
		Model("resource/model/wall/wall.obj"),
		BoxCollider(glm::vec3(5.8f, 2.0f, 0.5f))    // ������ü heat_box (x,y,z)
		));

	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(8.6f, -0.9f, -30.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(90.0f), 0.0f)),
			glm::vec3(6.99f, 5.0f, 1.0f)),
		Model("resource/model/wall/wall.obj"),
		BoxCollider(glm::vec3(5.8f, 2.0f, 0.5f))   // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(-8.6f, -0.9f, 30.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(90.0f), 0.0f)),
			glm::vec3(6.99f, 5.0f, 1.0f)),
		Model("resource/model/wall/wall.obj"),
		BoxCollider(glm::vec3(5.8f, 2.0f, 0.5f))    // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(8.6f, -0.9f, 30.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(90.0f), 0.0f)),
			glm::vec3(6.99f, 5.0f, 1.0f)),
		Model("resource/model/wall/wall.obj"),
		BoxCollider(glm::vec3(5.8f, 2.0f, 0.5f))    // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(30.0f, -0.9f, 8.6f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), 0.0f)),
			glm::vec3(6.99f, 5.0f, 1.0f)),
		Model("resource/model/wall/wall.obj"),
		BoxCollider(glm::vec3(5.8f, 2.0f, 0.5f))    // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(30.0f, -0.9f, -8.6f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), 0.0f)),
			glm::vec3(6.99f, 5.0f, 1.0f)),
		Model("resource/model/wall/wall.obj"),
		BoxCollider(glm::vec3(5.8f, 2.0f, 0.5f))    // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(-30.0f, -0.9f, 8.6f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), 0.0f)),
			glm::vec3(6.99f, 5.0f, 1.0f)),
		Model("resource/model/wall/wall.obj"),
		BoxCollider(glm::vec3(5.8f, 2.0f, 0.5f))    // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(-30.0f, -0.9f, -8.6f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), 0.0f)),
			glm::vec3(6.99f, 5.0f, 1.0f)),
		Model("resource/model/wall/wall.obj"),
		BoxCollider(glm::vec3(5.8f, 2.0f, 0.5f))    // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(20.0f, -0.9f, 20.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), 0.0f)),
			glm::vec3(0.5f, 5.0f, 0.3f)),
		Model("resource/model/wall/wall_new.obj"),
		BoxCollider(glm::vec3(20.0f, 2.0f, 2.7f))    // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(40.0f, -0.9f, 20.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), 0.0f)),
			glm::vec3(0.5f, 5.0f, 0.5f)),
		Model("resource/model/wall/wall_new.obj"),
		BoxCollider(glm::vec3(20.0f, 2.0f, 2.7f))    // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(15.3f, -0.9f, 25.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(90.0f), 0.0f)),
			glm::vec3(0.5f, 5.0f, 0.3f)),
		Model("resource/model/wall/wall_new.obj"),
		BoxCollider(glm::vec3(20.0f, 2.0f, 2.7f))    // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(44.7f, -0.9f, 25.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(90.0f), 0.0f)),
			glm::vec3(0.5f, 5.0f, 0.3f)),
		Model("resource/model/wall/wall_new.obj"),
		BoxCollider(glm::vec3(20.0f, 2.0f, 2.7f))    // ������ü heat_box (x,y,z)
		));
	// -----------------------------------------------
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(20.0f, -0.9f, 40.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), 0.0f)),
			glm::vec3(0.5f, 5.0f, 0.3f)),
		Model("resource/model/wall/wall_new.obj"),
		BoxCollider(glm::vec3(20.0f, 2.0f, 2.7f))    // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(40.0f, -0.9f, 40.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), 0.0f)),
			glm::vec3(0.5f, 5.0f, 0.5f)),
		Model("resource/model/wall/wall_new.obj"),
		BoxCollider(glm::vec3(20.0f, 2.0f, 2.7f))    // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(15.3f, -0.9f, 35.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(90.0f), 0.0f)),
			glm::vec3(0.5f, 5.0f, 0.3f)),
		Model("resource/model/wall/wall_new.obj"),
		BoxCollider(glm::vec3(20.0f, 2.0f, 2.7f))    // ������ü heat_box (x,y,z)
		));
	terrains.push_back(std::make_shared<Terrain>(
		Transform(
			glm::vec3(44.7f, -0.9f, 35.0f),
			glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(90.0f), 0.0f)),
			glm::vec3(0.5f, 5.0f, 0.3f)),
		Model("resource/model/wall/wall_new.obj"),
		BoxCollider(glm::vec3(20.0f, 2.0f, 2.7f))    // ������ü heat_box (x,y,z)
		));
}



/*void TerrainManager::loadCubemap()
{
	float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	// Texture
	// right, left, top, bottom, back, front
	std::vector<std::string> skyboxFaces = {
		"aaa"
	};

	glGenTextures(1, &skyboxID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);

	int width, height, nrChannels;
	// right, left, top, bottom, back, front
	for (unsigned int i = 0; i < skyboxFaces.size(); ++i)
	{
		unsigned char* data = stbi_load(skyboxFaces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cerr << "Cubemap texture load failed " << skyboxFaces[i] << std::endl;
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
}

void TerrainManager::drawCubemap() const
{
	glDepthFunc(GL_LEQUAL);
	skyboxShader.use();

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS); // set to default
}*/