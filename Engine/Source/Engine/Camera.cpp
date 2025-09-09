#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Core {

	Camera::Camera()
		: mProjectionMatrix(1.f)
		, mViewMatrix(1.f)
		, mAspectRatio(6.f / 4.f)
		, mHeight(1080.f)
		, mNear(0.f)
		, mFar(100.f)
	{
		RecalculateProjectionMatrix();
	}
	
	Camera::Camera(const Camera& other)
		: mAspectRatio(other.mAspectRatio)
		, mHeight(other.mHeight)
		, mNear(other.mNear)
		, mFar(other.mFar)
	{
		RecalculateProjectionMatrix();
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		mAspectRatio = aspectRatio;
		RecalculateProjectionMatrix();
	}

	void Camera::SetHeight(float height)
	{
		mHeight = height;
		RecalculateProjectionMatrix();
	}

	void Camera::setNear(float near)
	{
		mNear = near;
		RecalculateProjectionMatrix();
	}

	void Camera::SetFar(float far)
	{
		mFar = far;
		RecalculateProjectionMatrix();
	}

	void Camera::SetOrtho(float height, float near, float far)
	{
		mHeight = height;
		mNear = near;
		mFar = far;
		RecalculateProjectionMatrix();
	}

	void Camera::RecalculateProjectionMatrix()
	{
		float halfwidth = mHeight * mAspectRatio * 0.5f;
		float halfheight = mHeight * 0.5f;
		mProjectionMatrix = glm::ortho(-halfwidth, halfwidth, -halfheight, halfheight, mNear, mFar);
	}

	void Camera::SetViewMatrix(const glm::vec3& pos, float rot) 
	{
		glm::mat4 view = glm::mat4(1.f);
		view = glm::translate(view, pos);
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0, 0, 1));
		view = glm::inverse(view);
		SetViewMatrix(view);  
	}
}