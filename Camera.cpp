#include "Camera.h"


Camera::Camera(glm::vec3 position) 
{
	_position = position;
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);

	_right = glm::normalize(glm::cross(_front, _world_up));
	_up = glm::normalize(glm::cross(_right, _front));
}


glm::mat4 Camera::LookAt()const
{
	glm::mat4 rotation;
	rotation = glm::mat4(_right.x, _up.x, -_front.x, 0.0f,
						 _right.y, _up.y, -_front.y, 0.0f, 
						 _right.z, _up.z, -_front.z, 0.0f,
						 0.0f,     0.0f,  0.0f,      1.0f);

	glm::mat4 translation;
	translation[3][0] = -_position.x;
	translation[3][1] = -_position.y;
	translation[3][2] = -_position.z;
	return rotation * translation;

}


void Camera::move(Camera_Movement direction, float deltaTime)
{
	float velocity = _movement_speed * deltaTime;
	if (direction == FORWARD)
		_position += _front * velocity;
	if (direction == BACKWARD)
		_position -= _front * velocity;
	if (direction == UP)
		_position += _world_up * velocity;
	if (direction == DOWN)
		_position -= _world_up * velocity;
	if (direction == LEFT)
		_position -= _right * velocity;
	if (direction == RIGHT)
		_position += _right * velocity;
}


