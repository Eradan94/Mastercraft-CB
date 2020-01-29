#include <glm/glm.hpp>
#include <glimac/glm.hpp>
#include <math.h>

namespace glimac {

	class MasterCraftCamera {
	public:
	    MasterCraftCamera();
	    MasterCraftCamera(float x, float y, float z);
	    void computeDirectionVectors();

	    void maintainCameraInWorld(int limit);

	    void moveLeft(float t, int limit);
	    void moveFront(float t, int limit);
	    void moveAbove(float t, int limit);

	    void rotateLeft(float degrees);
	    void rotateUp(float degrees);

	    glm::vec3 getPosition();
	    glm::vec3 getUpVector();
	    glm::vec3 getFrontVector();
	    glm::vec3 getLeftVector();
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
