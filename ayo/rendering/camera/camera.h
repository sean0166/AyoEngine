#pragma once
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Ayo
{
	
	class Camera
	{
	public:
		Camera() = default;
		Camera(const Camera&) = default;
		Camera& operator=(const Camera&) = default;

		Camera(const glm::vec3& pos);
		Camera(const glm::vec3& pos, const glm::quat& orientation);
		
		
	private:
		glm::vec3 m_pos;
		glm::quat m_orientation;

	};
}