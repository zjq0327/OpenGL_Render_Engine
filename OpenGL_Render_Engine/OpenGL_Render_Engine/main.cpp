#include <iostream>

#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>//断言
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "camera/orthographicCamera.h"
#include "camera/perspectiveCamera.h"
#include "cameraControl/gameCameraControl.h"

#include"objects/model.h"
#include"objects/models/lamp.hpp"


GLuint vao;
Shader* shader = nullptr;
glm::mat4 transform(1.0f);
float size = 6.0f;
Camera* camera = nullptr;

GameCameraControl* cameraControl = nullptr;
void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		app->setWindowShouldClose();

	cameraControl->onKey(key, action, mods);
}

//鼠标按下/抬起
void OnMouse(int button, int action, int mods) {
	double x, y;
	app->getCursorPosition(&x, &y);
	cameraControl->onMouse(button, action, x, y);
}

//鼠标移动
void OnCursor(double xpos, double ypos) {
	cameraControl->onCursor(xpos, ypos);
}

//鼠标滚轮
void OnScroll(double offset) {
	cameraControl->onScroll(offset);
}



void prepareCamera() {
	float size = 6.0f;
	//camera = new OrthographicCamera(-size, size, size, -size, size, -size);
	camera = new PerspectiveCamera(
		60.0f,
		(float)app->getWidth() / (float)app->getHeight(),
		0.1f,
		1000.0f
	);
	cameraControl = new GameCameraControl();
	cameraControl->setCamera(camera);
	cameraControl->setSensitivity(0.4f);
}




int main() {
	if (!app->init(800, 600)) {
		return -1;
	}

	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);
	app->setMouseCallback(OnMouse);
	app->setCursorCallback(OnCursor);
	app->setScrollCallback(OnScroll);

	//设置opengl视口以及清理颜色
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));


	prepareCamera();


	// SHADERS===============================
	Shader shader("assets/shaders/phongshaders/vertex.glsl", "assets/shaders/phongshaders/fragment.glsl");
	Shader lampShader("assets/shaders/lightshaders/vertex.glsl", "assets/shaders/lightshaders/fragment.glsl");

	// MODELS==============================
	Model m(glm::vec3(0.0f, -2.0f, -5.0f), glm::vec3(0.05f), true);
	m.loadModel("assets/models/mary/Marry.obj");

	// LIGHTS
	DirLight dirLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) };

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	Lamp lamps[4];
	for (unsigned int i = 0; i < 4; i++) {
		lamps[i] = Lamp(glm::vec3(1.0f),
			glm::vec4(0.05f, 0.05f, 0.05f, 1.0f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), glm::vec4(1.0f),
			1.0f, 0.07f, 0.032f,
			pointLightPositions[i], glm::vec3(0.25f));
		lamps[i].init();
	}

	SpotLight s = {
		camera->mPosition, camera->mRight,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)),
		1.0f, 0.07f, 0.032f,
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f), glm::vec4(1.0f)
	};



	while (app->update()) {
		cameraControl->update();

		shader.begin();

		shader.set3Float("viewPos", camera->mPosition);

		dirLight.render(shader);

		for (unsigned int i = 0; i < 4; i++) {
			lamps[i].pointLight.render(shader, i);
		}
		shader.setInt("noPointLights", 4);

		
		shader.setInt("noSpotLights", 0);


		shader.setMat4("view", camera->getViewMatrix());
		shader.setMat4("projection", camera->getProjectionMatrix());

		m.render(shader);

		lampShader.begin();
		lampShader.setMat4("view", camera->getViewMatrix());
		lampShader.setMat4("projection", camera->getProjectionMatrix());

		for (unsigned int i = 0; i < 4; i++) {
			lamps[i].render(lampShader);
		}



	}

	app->destroy();

	return 0;
}