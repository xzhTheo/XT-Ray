#include "ray.h"

Ray::Ray(glm::vec3 src, glm::vec3 dest) : _origin(src), _direction(glm::normalize(dest - src))
{

}

glm::vec3 Ray::rayGo(float t) const
{
    return _origin + t * _direction;
}