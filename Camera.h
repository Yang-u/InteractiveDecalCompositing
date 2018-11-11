#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	UP,
	DOWN,
	LEFT,
	RIGHT
};
class Camera
{
public:

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 GetViewMatrix()const;
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

private:
	void updateCameraVectors();

public:

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp = glm::vec3(0.f, 1.f, 0.f);

	float Yaw = -90.f;
	float Pitch = 0.f;
	float Zoom = 45.f;
	float MovementSpeed = 5.f;

};

