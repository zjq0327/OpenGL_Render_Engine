#pragma once

#include"glframework/core.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "glframework/shader.h"


#include "mesh.h"


class Model {
public:
	glm::vec3 pos;
	glm::vec3 size;

	Model() {}
	Model(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool noTex = false);

	void loadModel(std::string path);

	void render(Shader shader);

	void cleanup();

protected:
	bool noTex;

	std::vector<Mesh> meshes;
	std::string directory;

	std::vector<Texture> textures_loaded;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type);
};