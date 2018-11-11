#include "Camera.h"


Camera::Camera(glm::vec3 position) 
{
	Position = position;
	updateCameraVectors();
}


glm::mat4 Camera::GetViewMatrix()const
{
	glm::mat4 rotation;
	rotation = glm::mat4(Right.x, Up.x, -Front.x, 0.0f, Right.y, Up.y, -Front.y, 0.0f, Right.z, Up.z, -Front.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 translation;
	translation[3][0] = -Position.x;
	translation[3][1] = -Position.y;
	translation[3][2] = -Position.z;
	return rotation * translation;

}


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == UP)
		Position += WorldUp * velocity;
	if (direction == DOWN)
		Position -= WorldUp * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}



void Camera::updateCameraVectors()
{

	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}


