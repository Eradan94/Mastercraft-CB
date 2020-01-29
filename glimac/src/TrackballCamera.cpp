#include "glimac/TrackballCamera.hpp"

namespace glimac {

	TrackballCamera::TrackballCamera(float m_fDistance, float m_fAngleX, float m_fAngleY) :
	m_fDistance(m_fDistance), m_fAngleX(m_fAngleX), m_fAngleY(m_fAngleY) {

	}

	void TrackballCamera::moveFront(float delta) {
		m_fDistance += delta;
	}
	void TrackballCamera::rotateLeft(float degrees) {
		m_fAngleX += degrees;
	}
	void TrackballCamera::rotateUp(float degrees) {
		m_fAngleY += degrees;
	}

	glm::mat4 TrackballCamera::getViewMatrix() const {
		glm::mat4 camera(1.f);
		camera = glm::translate(camera, glm::vec3(0, 0, m_fDistance)); // on distance de la caméra
		camera = glm::rotate(camera, (m_fAngleY * glm::pi<float>()) / 180, glm::vec3(1, 0, 0));
		camera = glm::rotate(camera, (m_fAngleX * glm::pi<float>()) / 180, glm::vec3(0, 1, 0));
		return camera;
	}

}
