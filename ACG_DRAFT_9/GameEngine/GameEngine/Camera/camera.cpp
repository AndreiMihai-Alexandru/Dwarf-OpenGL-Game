#include "camera.h"



Camera::Camera(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
	this->rotationOx = 0.0f;
	this->rotationOy = -90.0f;
}

Camera::Camera()
{
	//chnage player position
	this ->cameraPosition = glm::vec3(345, 0.0f, -1250);
	this ->cameraViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	this ->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
	this->rotationOx = 0.0f;
	this->rotationOy = -90.0f;
}

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraViewDirection, glm::vec3 cameraUp)
{
	this->cameraPosition = cameraPosition;
	this->cameraViewDirection = cameraViewDirection;
	this->cameraUp = cameraUp;
	this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

Camera::~Camera()
{
}

void Camera::keyboardMoveFront(float cameraSpeed)
{
	cameraPosition += cameraViewDirection * cameraSpeed;
}

void Camera::keyboardMoveBack(float cameraSpeed)
{
	cameraPosition -= cameraViewDirection * cameraSpeed;
}

void Camera::keyboardMoveLeft(float cameraSpeed)
{
	cameraPosition -= cameraRight * cameraSpeed;
}

void Camera::keyboardMoveRight(float cameraSpeed)
{
	cameraPosition += cameraRight * cameraSpeed;
}


void Camera::keyboardMoveUp(float cameraSpeed)
{
	cameraPosition += cameraUp * cameraSpeed;
}

void Camera::keyboardMoveDown(float cameraSpeed)
{
	cameraPosition -= cameraUp * cameraSpeed;
}

void Camera::rotateOx(float angle)
{	
	cameraViewDirection = glm::normalize(glm::vec3((glm::rotate(glm::mat4(1.0f), angle, cameraRight) * glm::vec4(cameraViewDirection, 1))));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraViewDirection));
	cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

void Camera::rotateOy(float angle)
{
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	cameraViewDirection = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(cameraViewDirection, 1.0f)));
	cameraRight = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(cameraRight, 1.0f)));
	cameraUp = glm::cross(cameraRight, cameraViewDirection);
}

void Camera::jump()
{
	if (!isJumping)
	{
		isJumping = true;
		cameraPosition.y += jumpHeight;
	}
}

void Camera::superjump()
{
	if (!isJumping)
	{
		isJumping = true;
		cameraPosition.y += superjumpHeight;
	}

}

float Camera::sprint(bool shouldSprint, float cameraSpeed)
{
	if (shouldSprint)
	{
		cameraSpeed *= sprintMultiplier;

		return cameraSpeed;
	}
}


void Camera::update(float deltaTime)
{
	// Simulate gravity
		cameraPosition.y -= gravity * deltaTime;
		if (cameraPosition.y < 0.0f)
		{
			cameraPosition.y = 0.0f;
			isJumping = false;
		}

		printCurrentPosition();
}


//teleport player 
void Camera::setCameraPosition(glm::vec3 cameraPosition)
{
	this->cameraPosition = cameraPosition;
}


glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(cameraPosition, cameraPosition + cameraViewDirection, cameraUp);
}

glm::vec3 Camera::getCameraPosition()
{
	return cameraPosition;
}

glm::vec3 Camera::getCameraViewDirection()
{
	return cameraViewDirection;
}

glm::vec3 Camera::getCameraUp()
{
	return cameraUp;
}




