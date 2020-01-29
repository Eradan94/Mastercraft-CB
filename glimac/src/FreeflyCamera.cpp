#include <iostream>

#include <cmath>

#include "glimac/FreeflyCamera.hpp"


namespace glimac {

	FreeflyCamera::FreeflyCamera() {
		m_Position = glm::vec3(0.f, 0.f, 0.f);
		m_fPhi = glm::pi<float>();
		m_fTheta = 0;
		computeDirectionVectors();
	}

	// Constructor with player's position
	FreeflyCamera::FreeflyCamera(float x, float y, float z) {
		m_Position = glm::vec3(x, y, z);
		m_fPhi = glm::pi<float>();
		m_fTheta = 0;
		computeDirectionVectors();
	}

	void FreeflyCamera::computeDirectionVectors() {
		//F⃗ =(cos(θ)sin(ϕ), sin(θ), cos(θ)cos(ϕ))
		m_FrontVector = glm::vec3(cos(m_fTheta) * sin(m_fPhi), sin(m_fTheta), cos(m_fTheta) * cos(m_fPhi));
		//L⃗ =(sin(ϕ+π2), 0, cos(ϕ+π2))
		m_LeftVector = glm::vec3(sin(m_fPhi + glm::pi<float>()/2), 0.f, cos(m_fPhi + glm::pi<float>()/2));
		//U = F * L;
		m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
	}

	void FreeflyCamera::moveLeft(float t) {
		m_Position += t * m_LeftVector;
	}
	void FreeflyCamera::moveFront(float t) {
		m_Position += t * m_FrontVector;
	}

	void FreeflyCamera::rotateLeft(float degrees) {
		m_fPhi += (degrees * glm::pi<float>()) / 180.f;
	}
	void FreeflyCamera::rotateUp(float degrees) {
	    float radian = (degrees * glm::pi<float>()) / 180.f;
		if(m_fTheta + radian > 1.3) // Lock the rotation up 70 degrees (1.3 rad)
            m_fTheta = 1.3;
        else if(radian < 0 && m_fTheta + radian < -1.3) // Lock the rotation
            m_fTheta = -1.3;
        else
            m_fTheta += radian;
	}

	glm::vec3 FreeflyCamera::getPosition() {
	    return m_Position;
	}

	float FreeflyCamera::getPhi() {
	    return m_fPhi;
	}

	glm::mat4 FreeflyCamera::getViewMatrix() const {
		return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
	}

}
