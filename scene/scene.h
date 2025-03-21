

#ifndef RAY_SCENE_H
#define RAY_SCENE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shape.h"
#include <vector>
#include "common.h"

struct HitResult
{
    Shape* shape = nullptr;
    glm::vec3 hitPoint;
    glm::vec3 normal;
    float t;
    float distance;
};

class Scene
{
public:
    Scene();
    ~Scene();
    void addEntity(Shape* entity);
    void addLight(Light* light);
    glm::vec3 traceRay(const Ray& ray, unsigned int recursionTime = 0);
    HitResult getIntersection(const Ray& ray);
    glm::vec3 shade(const Shape& entity, glm::vec3 fragPos, const Ray& ray);

    static const unsigned int MAX_RECURSION_TIME;
private:
    std::vector<Shape*> _entitys;
    std::vector<Light*> _lights;
};

#endif