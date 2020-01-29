#include <glm/glm.hpp>
#include <glimac/glm.hpp>
#include <math.h>

namespace glimac {

	class FreeflyCamera {
	public:
	    FreeflyCamera();
	    FreeflyCamera(float x, float y, float z);
	    void computeDirectionVectors();

	    void moveLeft(float t);
	    void moveFront(float t);

	    void rotateLeft(float degrees);
	    void rotateUp(float degrees);

	    glm::vec3 getPosition();
	    float getPhi();

	    glm::mat4 getViewMatrix() const;

	private:
	    glm::vec3 m_Position;
	    float m_fPhi;
	    float m_fTheta;

	    glm::vec3 m_FrontVector;
	    glm::vec3 m_LeftVector;
	    glm::vec3 m_UpVector;
	};

}
