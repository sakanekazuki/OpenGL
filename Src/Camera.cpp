/**
* @file Camera.cpp
*/

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

/**
* 更新
*/
void Camera::Update()
{
	matProj =
		glm::perspective(glm::radians(45.0f), 
			aspectRatio, 0.1f, 200.0f);
	matView =
		glm::lookAt(position, target, glm::vec3(0, 0.1, 0));
}

/**
* ビュー行列を取得
*/
const glm::mat4& Camera::GetViewMatrix()const
{
	return matView;
}

/**
* プロジェクト行列を取得
*/
const glm::mat4& Camera::GetProjectMatrix()const
{
	return matProj;
}