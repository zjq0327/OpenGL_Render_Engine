#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>//����
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/texture.h"
#include "camera/orthographicCamera.h"


GLuint vao;
Shader* shader = nullptr;
Texture* texture = nullptr;
glm::mat4 transform(1.0f);
glm::mat4 viewMatrix(1.0f);
glm::mat4 perspectiveMatrix(1.0f);
float size = 6.0f;
Camera* camera = new OrthographicCamera(-size, size, size, -size, size, -size);


void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	std::cout << key << std::endl;
}

void prepareVAO() {
	//1 ׼��positions colors
	/*float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
	};*/
	float positions[] = {
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};


	float colors[] = {
		1.0f, 0.0f,0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 0.0f,1.0f,
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
	};

	//2 VBO����
	GLuint posVbo, colorVbo, uvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//3 EBO����
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO����
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//5 ��vbo ebo ��������������Ϣ
	//5.1 ����λ������������Ϣ
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.2 ������ɫ������������
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.3 ����uv������������
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	//5.4 ����ebo����ǰ��vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

void prepareShader() {
	shader = new Shader("assets/shaders/vertex.glsl","assets/shaders/fragment.glsl");
}

void prepareTexture() {
	texture = new Texture("assets/textures/goku.jpg", 0);
}

void prepareCamera() {

	viewMatrix = camera->getViewMatrix();
}

void preparePerspective() {

	perspectiveMatrix = camera->getProjectionMatrix();
}

void render() {
	//ִ��opengl������������
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//�󶨵�ǰ��program
	shader->begin();
	shader->setInt("sampler", 0);
	shader->setMatrix4x4("transform", transform);
	shader->setMatrix4x4("viewMatrix", viewMatrix);
	shader->setMatrix4x4("projectionMatrix", perspectiveMatrix);
 
	//�󶨵�ǰ��vao
	GL_CALL(glBindVertexArray(vao));

	//��������ָ��
	GL_CALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
	GL_CALL(glBindVertexArray(0));

	shader->end();
}


int main() {
	if (!app->init(800, 600)) {
		return -1;
	}

	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);

	//����opengl�ӿ��Լ�������ɫ
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	prepareShader();
	prepareVAO();
	prepareTexture();
	prepareCamera();
	preparePerspective();

	while (app->update()) {
		render();
	}

	app->destroy();

	return 0;
}