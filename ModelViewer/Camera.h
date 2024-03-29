#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <glcorearb.h>
#include <glm/glm.hpp>
#include "RenderData.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
private:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	float lastX = FrameInfo::ScreenWidth / 2.0f;
	float lastY = FrameInfo::ScreenHeight / 2.0f;

	bool firstMouse = true;

	void updateCameraVectors();
public:

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.5f, 2.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	glm::vec3 getFront();
	glm::vec3 getPostion();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void processInput(float deltaTime);
	void processMouse(float xoffset, float yoffset);
	void processScroll(float yoffset);
	
};

#endif