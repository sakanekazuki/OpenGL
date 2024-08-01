/**
* @file Camera.h
*/

#ifndef CAMERA_H_INCLUDE
#define CAMERA_H_INCLUDE

#include "Actor/Actor.h"
#include "ProgramPipeline.h"

class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	void Update();
	
	const glm::mat4& GetViewMatrix()const;
	const glm::mat4& GetProjectMatrix()const;

	// カメラの座標
	glm::vec3 position = glm::vec3(0);
	// カメラの注視点の座標
	glm::vec3 target = glm::vec3(0);
	
	// カメラの回転
	glm::mat4 rotation_y = glm::mat4(1);
	glm::vec3 rotation_y_v = glm::vec3(0, 0, 1);
	glm::mat4 rotation_x = glm::mat4(1);
	glm::vec3 rotation_x_v = glm::vec3(1, 0, 0);
	float Angle_y = 0;
	float Angle_x = 0;

	float aspectRatio = 1.0f;

private:
	glm::mat4 matProj = glm::mat4(1);
	glm::mat4 matView = glm::mat4(1);

};

#endif // CAMERA_H_INCLUDE