#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <string>
#include <utility>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Camera.h"
#include "Shader.h"
#include "Model.h"

void cursorPos_callback(GLFWwindow* window, double xpos, double ypos);
void mouseButton_callback(GLFWwindow *window, int button, int action, int modifiers);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int genTexture(const string& path);

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));


double Width = 1200, Height = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float Right_lastX = Width / 2.0f;
float Right_lastY = Height / 2.0f;
bool firstMouse_Right = true;

bool polygonMode = false;

Shader shader("shader\\shader.vs", "shader\\shader.fs");

Eigen::Vector3d ProjectOnPlane(Eigen::Vector3d p, Eigen::Vector3d n, Eigen::Vector3d q) {
	if (((q - p).norm()) == 0)return q;
	else return (q - p) - (q - p).dot(n) * n;
}

int main()
{
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

	shader.init();
	Model model("objects//sphere-4k.obj",1);
	unsigned int texture = genTexture("texture//Chess_Board.png");

	

	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		glfwPollEvents();
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective<float>(glm::radians(camera.Zoom), Width / Height, 0.1f, 100.0f);
		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("direction", camera.Front);
		model.draw(shader);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

unsigned int genTexture(const string& path) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

}

void cursorPos_callback(GLFWwindow* window, double xpos, double ypos) {

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		if (firstMouse_Right) {
			Right_lastX = xpos;
			Right_lastY = ypos;
			firstMouse_Right = false;
		}

		float xoffset = xpos - Right_lastX;
		float yoffset = Right_lastY - ypos;
		Right_lastX = xpos;
		Right_lastY = ypos;
		camera.ProcessMouseMovement(xoffset, yoffset);
	}

}

void mouseButton_callback(GLFWwindow *window, int button, int action, int modifiers) {

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		firstMouse_Right = true;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Width = width;
	Height = height;
	glViewport(0, 0, width, height);

}



