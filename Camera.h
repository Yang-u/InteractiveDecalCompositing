#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
	FORWARD=0,
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
	glm::mat4 LookAt()const;
	void move(Camera_Movement direction, float deltaTime);

public:
	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _world_up = glm::vec3(0.f, 1.f, 0.f);

	float _yaw = -90.f;
	float _pitch = 0.f;
	float _zoom = 45.f;
	float _movement_speed = 5.f;
};

