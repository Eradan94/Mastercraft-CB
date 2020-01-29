#include <glm/glm.hpp>
#include <glimac/glm.hpp>
#include <math.h>

namespace glimac {

    class TrackballCamera {
    public:
        TrackballCamera(float m_fDistance = -5.f, float m_fAngleX = 0.f, float m_fAngleY = 0.f);

        void moveFront(float delta);
        void rotateLeft(float degrees);
        void rotateUp(float degrees);

        glm::mat4 getViewMatrix() const;

    private:
        float m_fDistance;
        float m_fAngleX;
        float m_fAngleY;
    };
    
}