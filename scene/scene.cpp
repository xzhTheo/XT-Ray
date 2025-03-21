﻿#include "scene.h"
#include <iostream>

const unsigned int Scene::MAX_RECURSION_TIME = 5;

Scene::Scene()
{

}
Scene::~Scene()
{
    for (auto ptr : _entitys)
    {
        delete ptr;
    }
    for (auto ptr : _lights)
    {
        delete ptr;
    }
}
void Scene::addEntity(Shape* entity)
{
    _entitys.push_back(entity);
}
void Scene::addLight(Light* light)
{
    _lights.push_back(light);
}
glm::vec3 Scene::traceRay(const Ray& ray, unsigned int recursionTime)
{
    glm::vec3 lightIntensity(0.0f);  // 用于返回的光线强度，初始化为0

	// 递归结束条件：超过最大递归次数
    if (recursionTime >= MAX_RECURSION_TIME)
    {
        return glm::vec3{0, 0, 0};
    }

	// 计算光线与物体的交点以及该物体
    const auto hitResult = getIntersection(ray);
    const Shape* collidedEntityPtr = hitResult.shape;

   // 递归结束条件：光线没有照射到物体上
    if (collidedEntityPtr == nullptr)
    {
        // background color
        return glm::vec3{1, 1, 1};
    }

    // 获得照射点及其法向量
    glm::vec3 collidedPoint =  hitResult.hitPoint;
    // std::cout << "collidedPoint: " << collidedPoint << std::endl;
    bool enterEntity = collidedEntityPtr->rayInEntity(ray);
    // glm::vec3 normal = glm::normalize(collidedEntityPtr->calNormal(collidedPoint));
    // if (enterEntity)
    // {
    //     // 若光线是从物体内部射出的，法向量应该取反
    //     normal = -normal;
    // }

    // 光照强度的第一部分：局部光照强度
    if (!enterEntity)
    {
        lightIntensity = collidedEntityPtr->getMaterial().kShade *
            shade(*collidedEntityPtr, collidedPoint, ray);
    }
    

    // // 计算反射方向
    // glm::vec3 reflectDirection = glm::reflect(ray.getDirection(), normal);
    
    // // 光照强度的第二部分：反射光照强度
    // if (collidedEntityPtr->getMaterial().kReflect > FLOAT_EPS) // > 0
    // {
    //     std::cout <<collidedEntityPtr->getName() <<  reflectDirection << "collidedPoint" << collidedPoint << std::endl;
    //     lightIntensity += collidedEntityPtr->getMaterial().kReflect *
    //         traceRay(Ray(collidedPoint, collidedPoint + reflectDirection), recursionTime + 1);
    // }

    // // 计算折射率
    // float currentIndex = 1.0f;
    // float nextIndex = collidedEntityPtr->getMaterial().refractiveIndex;
    // if (enterEntity)
    // {
    //     // 若光线是从物体内部射出的，折射率需要进行交换
    //     std::swap(currentIndex, nextIndex);
    // }

    // // 计算折射方向
    // glm::vec3 refractDirection = glm::refract(ray.getDirection(), normal,  currentIndex / nextIndex);

    // // 光照强度的第三部分：折射光照强度
    // if (collidedEntityPtr->getMaterial().kRefract > FLOAT_EPS) // > 0
    // {
    //     lightIntensity += collidedEntityPtr->getMaterial().kRefract * 
    //         traceRay(Ray(collidedPoint, collidedPoint + refractDirection), recursionTime + 1);
    // }

    return lightIntensity;
}

HitResult Scene::getIntersection(const Ray& ray)
{
    HitResult result;
    float minT = FLOAT_INF;
    const Shape* collidedEntity = nullptr;
    for (auto pEntity : _entitys)
    {
        float t = pEntity->rayCollision(ray);
        if (t > FLOAT_EPS) // >= 0
        {
            // std::cout << pEntity->getName() <<"float t = pEntity->rayCollision(ray)" << t << std::endl;
            if (t < minT)
            {
                result.hitPoint = ray.rayGo(t);
                result.t = t;
                result.shape = pEntity;
                result.normal = glm::normalize(pEntity->calNormal(result.hitPoint));
                result.distance = glm::distance(ray.getOrigin(), result.hitPoint);
                minT = t;
            }				
        }
    }
    // std::cout << "minT: " << minT << "ray.rayGo(minT) " << ray.rayGo(minT) << std::endl;
    return result;
}

glm::vec3 Scene::shade(const Shape& entity, glm::vec3 fragPos, const Ray& ray)
{
    glm::vec3 result(0.0f);
    for (auto pLight : _lights)
    {
        if(entity.getName() == "Plane")
        {
            std::cout << result << "normal: "<< entity.calNormal(fragPos)  << std::endl;
            std::cout <<  "fragPos: "<< fragPos << " ray.getDirection() : " << ray.getDirection() << std::endl;
            result += pLight->calLight(entity.getMaterial(), fragPos, entity.calNormal(fragPos), ray.getDirection());

        }
        // std::cout << result << "normal: "<< entity.calNormal(fragPos)  << std::endl;
        // std::cout <<  "fragPos: "<< fragPos << " ray.getDirection() : " << ray.getDirection() << std::endl;
    }
    return result;
}

