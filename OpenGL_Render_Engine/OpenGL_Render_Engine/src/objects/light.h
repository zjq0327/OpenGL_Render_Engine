#pragma once


#include <glm/glm.hpp>

#include "glframework/shader.h"

struct DirLight {
	glm::vec3 direction;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void render(Shader shader);
};

struct PointLight {
	glm::vec3 position;

	// attenuation constants
	float k0;
	float k1;
	float k2;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void render(Shader shader, int idx);
};

struct SpotLight {
	glm::vec3 position;
	glm::vec3 direction;

	float cutOff;
	float outerCutOff;

	// attenuation constants
	float k0;
	float k1;
	float k2;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	void render(Shader shader, int idx);
};

