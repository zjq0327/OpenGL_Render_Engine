#pragma once
#include "glframework/core.h"
#include <string>

#include "assimp/scene.h"


class Texture {
public:
	Texture(std::string dir, std::string path, aiTextureType type);

	void generate();
	void load(bool flip = true);

	void bind();

	// texture object
	unsigned int id;
	aiTextureType type;
	std::string dir;
	std::string path;
};