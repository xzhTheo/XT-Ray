#ifndef RAY_TRACING_RAY_H
#define RAY_TRACING_RAY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "common.h"
static const float FLOAT_INF = 100000000.0f;
static const float FLOAT_EPS = 1e-5;
static const glm::vec3 NULL_POINT(FLOAT_INF, FLOAT_INF, FLOAT_INF);
class Ray
{
public:
    Ray(glm::vec3 src, glm::vec3 dest);
    glm::vec3 rayGo(float t) const;
    glm::vec3 getOrigin() const { return _origin; }
    glm::vec3 getDirection() const { return _direction; }
private:
    glm::vec3 _origin;
    glm::vec3 _direction;
};


#endif