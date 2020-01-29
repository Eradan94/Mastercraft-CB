#include <iostream>

#include <cmath>

#include "glimac/MasterCraftCamera.hpp"


namespace glimac {

	MasterCraftCamera::MasterCraftCamera() {
		m_Position = glm::vec3(0.f, 0.f, 0.f);
		m_fPhi = glm::pi<float>();
		m_fTheta = 0;
		computeDirectionVectors();
	}

	// Constructor with player's position
	MasterCraftCamera::MasterCraftCamera(float x, float y, float z) {
		m_Position = glm::vec3(x, y, z);
		m_fPhi = glm::pi<float>();
		m_fTheta = 0;
		computeDirectionVectors();
	}

	void MasterCraftCamera::computeDirectionVectors() {
		//F⃗ =(cos(θ)sin(ϕ), sin(θ), cos(θ)cos(ϕ))
		m_FrontVector = glm::vec3(cos(m_fTheta) * sin(m_fPhi), sin(m_fTheta), cos(m_fTheta) * cos(m_fPhi));
		//L⃗ =(sin(ϕ+π2), 0, cos(ϕ+π2))
		m_LeftVector = glm::vec3(sin(m_fPhi + glm::pi<float>()/2), 0.f, cos(m_fPhi + glm::pi<float>()/2));
		//U = F * L;
		m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
	}

	void MasterCraftCamera::maintainCameraInWorld(int limit) {
	    if(m_Position.x < 0.) {
            m_Position.x = 0;
		}
		if(m_Position.x > limit) {
            m_Position.x = limit;
		}
		if(m_Position.y > 255.) {
            m_Position.y = 255.;
		}
		if(m_Position.z > limit) {
            m_Position.z = limit;
		}
		if(m_Position.z < 0.) {
            m_Position.z = 0.;
		}
	}

	void MasterCraftCamera::moveLeft(float t, int limit) {
	    //float lastYPos = m_Position.y;
		m_Position += t * m_LeftVector;
		maintainCameraInWorld(limit);
		//m_Position.y = lastYPos;
	}
	void MasterCraftCamera::moveFront(float t, int limit) {
	    //float lastYPos = m_Position.y;
	    m_Position += t * m_FrontVector;
        maintainCameraInWorld(limit);
        //m_Position.y = lastYPos;
	}

	void MasterCraftCamera::moveAbove(float t, int limit) {
        m_Position.y += t;
        maintainCameraInWorld(limit);
	}

	void MasterCraftCamera::rotateLeft(float degrees) {
		m_fPhi += (degrees * glm::pi<float>()) / 180.f;
	}
	void MasterCraftCamera::rotateUp(float degrees) {
	    float radian = (degrees * glm::pi<float>()) / 180.f;
		if(m_fTheta + radian > 1.3) // Lock the rotation up 70 degrees (1.3 rad)
            m_fTheta = 1.3;
        else if(radian < 0 && m_fTheta + radian < -1.3) // Lock the rotation
            m_fTheta = -1.3;
        else
            m_fTheta += radian;
	}

	glm::vec3 MasterCraftCamera::getPosition() {
	    return m_Position;
	}

	glm::vec3 MasterCraftCamera::getUpVector() {
	    return m_UpVector;
	}

	glm::vec3 MasterCraftCamera::getFrontVector() {
	    return m_FrontVector;
	}

	glm::vec3 MasterCraftCamera::getLeftVector() {
	    return m_LeftVector;
	}

	float MasterCraftCamera::getPhi() {
	    return m_fPhi;
	}

	glm::mat4 MasterCraftCamera::getViewMatrix() const {
		return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
	}

}
