#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <nanogui/nanogui.h>
#include <nanogui/imagepanel.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <string>
#include <utility>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Camera.h"
#include "Shader.h"
#include "Screen.h"
#include "ModelRenderer.h"

void cursorPos_callback(GLFWwindow* window, double xpos, double ypos);
void mouseButton_callback(GLFWwindow *window, int button, int action, int modifiers);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);



double Width = 1200, Height = 600;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float Right_lastX = Width / 2.0f,Left_lastX= Width / 2.0f;
float Right_lastY = Height / 2.0f, Left_lastY = Height / 2.0f;
bool firstMouse_Right = true;
bool firstMouse_Left = true;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
Screen screen;
FractalRenderer fractalRenderer;
ModelRenderer modelRenderer;
Shader shader("shader/shader.vs", "shader/shader.fs");
Shader colorShader("shader/colorShader.vs", "shader/colorShader.fs");

int main(int argc,char* argv[])
{
	string modelPath("objects/sphere-4k.obj");
	int* index = nullptr;
	int length = 0;
	if (argc > 2) {
		modelPath = argv[1];
		length = argc - 2;
		index = new int[length];
		for (int i = 2; i < argc; ++i) {
			index[i - 2] = atoi(argv[i]);
		}
	}
	glfwInit();

	glfwSetTime(0);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(Width, Height, "DEM", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Could not initialize GLAD!");


	glfwSetMouseButtonCallback(window, mouseButton_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursorPos_callback);
	glfwSetScrollCallback(window, scroll_callback);

	shader.initialize();
	colorShader.initialize();
	Model model(modelPath,index,length);
	modelRenderer.initialize(&model);
	fractalRenderer.initialize(600, 600);
	fractalRenderer.renderFractal(0, 0, 0,0,0);
	screen.initialize(window, false);


	while (!glfwWindowShouldClose(window))
	{
		
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		glfwPollEvents();
		
		glViewport(0,0,Width, Height);
		modelRenderer.render(shader);
		screen.update();
		screen.getScreen()->drawContents();
		screen.getScreen()->drawWidgets();
		
		//int width, height, nrChannels;
		//unsigned int tex = fractalRenderer.getTexture(&width, &height, &nrChannels);
		////unsigned int tex = Texture::genTexture("texture/pic.png", &width, &height, &nrChannels);
		//Texture::saveTexture(width, height, nrChannels, tex, "test");
		glfwSwapBuffers(window);
		//break;
	}

	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.move(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.move(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.move(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.move(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.move(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.move(RIGHT, deltaTime);

}

void mouseButton_callback(GLFWwindow *window, int button, int action, int modifiers) {
	screen.getScreen()->mouseButtonCallbackEvent(button, action, modifiers);

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (action == GLFW_PRESS) {
		modelRenderer.renderOffScreen(colorShader, x, y);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		firstMouse_Left = true;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		firstMouse_Right = true;
}

void cursorPos_callback(GLFWwindow* window, double xpos, double ypos) {
	screen.getScreen()->cursorPosCallbackEvent(xpos, ypos);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
		if (firstMouse_Left) {
			Left_lastX = xpos;
			Left_lastY = ypos;
			firstMouse_Left = false;
		}
		float xoffset = xpos - Left_lastX;
		float yoffset = Left_lastY - ypos;
		Left_lastX = xpos;
		Left_lastY = ypos;
		if (modelRenderer.isSelect()) {
			xoffset *= 0.01;
			yoffset *= 0.01;
			Model* model = modelRenderer.getModel();
			model->setPosition(model->getPosition() + camera._right*xoffset + camera._up*yoffset);
		}
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		if (firstMouse_Right) {
			Right_lastX = xpos;
			Right_lastY = ypos;
			firstMouse_Right = false;
		}

		float xoffset = xpos - Right_lastX;
		float yoffset = Right_lastY - ypos;
		Right_lastX = xpos;
		Right_lastY = ypos;
		if (modelRenderer.isSelect()) {
			xoffset *= 0.01;
			yoffset *= 0.01;
			Model* model = modelRenderer.getModel();

			model->setRotate(xoffset, camera._up);
			model->setRotate(yoffset, -camera._right);
		}
	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (modelRenderer.isSelect()) {
		Model* model = modelRenderer.getModel();
		model->setScale(model->getScale() + glm::vec3(yoffset * 0.05));
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Width = width;
	Height = height;
	glViewport(0, 0, width, height);

}



