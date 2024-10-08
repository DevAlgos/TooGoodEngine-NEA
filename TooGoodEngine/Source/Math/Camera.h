#pragma once

#include <glm/glm.hpp>

namespace TooGoodEngine {

	//this is to be inherited by every type of camera (mainly orthographic and perspective)
	class Camera
	{
	public:
		virtual const glm::vec3& GetCameraPosition() = 0;
		virtual const glm::mat4& GetProjection() = 0;
		virtual const glm::mat4& GetView() = 0;
		virtual const glm::mat4& GetInverseProjection() = 0;
		virtual const glm::mat4& GetInverseView() = 0;
		virtual void OnWindowResize(float newWidth, float newHeight) = 0;
	};

}