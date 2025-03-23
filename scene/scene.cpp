#include "scene.h"
#include <iostream>

const unsigned int Scene::MAX_RECURSION_TIME = 5;

Scene::Scene()
{
    count = 0;
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

/*
glm::vec3 lightIntensity(0.0f);  // 用于返回的光线强度，初始化为0

    // 递归结束条件：超过最大递归次数
    if (recursionTime >= MAX_RECURSION_TIME)
    {
        return glm::vec3{0, 0, 0};
    }

    // 计算光线与物体的交点以及该物体
    auto hitResult = getIntersection(ray);
    const Shape* collidedEntityPtr = hitResult.shape;

   // 递归结束条件：光线没有照射到物体上
    if (hitResult.shape == nullptr)
    {
        // background color
        return glm::vec3{1, 1, 1};
    }
    bool enterEntity = collidedEntityPtr->rayInEntity(ray);
     if (enterEntity)
     {
         // 若光线是从物体内部射出的，法向量应该取反
        hitResult.normal = -hitResult.normal;
     }
     //hitResult.hitPoint = hitResult.hitPoint + hitResult.normal * 0.1f; // 防止光线与自己相交

    // 光照强度的第一部分：局部光照强度
    if (!enterEntity)
    {
        glm::vec3 shaderColor = shade(hitResult, ray);
        lightIntensity = collidedEntityPtr->getMaterial().kShade_ * shaderColor;
        if(lightIntensity.x > 0)
        {
            // std::cout << "lightIntensity" << lightIntensity << std::endl;
        }

        //std::cout <<"shaderColor" << shaderColor << "collidedEntityPtr->getMaterial().kShade_" << collidedEntityPtr->getMaterial().kShade_ << std::endl;
    }


    // // 计算反射方向
     glm::vec3 reflectDirection = glm::reflect(ray.getDirection(), hitResult.normal);

     // 光照强度的第二部分：反射光照强度
     if (collidedEntityPtr->getMaterial().kReflect_ > FLOAT_EPS) // > 0
     {
         //std::cout <<collidedEntityPtr->getName() <<  reflectDirection << "collidedPoint" << hitResult.hitPoint << std::endl;
         lightIntensity += collidedEntityPtr->getMaterial().kReflect_ *
             traceRay(Ray(hitResult.hitPoint + hitResult.normal * 0.1f,  reflectDirection), recursionTime + 1);
     }

    // // 计算折射率
     float currentIndex = 1.0f;
     float nextIndex = collidedEntityPtr->getMaterial().refractiveIndex_;
     if (enterEntity)
     {
         // 若光线是从物体内部射出的，折射率需要进行交换
         std::swap(currentIndex, nextIndex);
     }

     // 计算折射方向
     glm::vec3 refractDirection = glm::refract(ray.getDirection(), hitResult.normal,  currentIndex / nextIndex);

     // 光照强度的第三部分：折射光照强度
     if (collidedEntityPtr->getMaterial().kRefract_ > FLOAT_EPS) // > 0
     {
         lightIntensity += collidedEntityPtr->getMaterial().kRefract_ *
             traceRay(Ray(hitResult.hitPoint - hitResult.normal * 0.1f, refractDirection), recursionTime + 1);
     }

    return lightIntensity;
*/
glm::vec3 Scene::traceRay(const Ray& ray, unsigned int recursionTime)
{
    glm::vec3 lightIntensity(0.0f);

    if (recursionTime >= MAX_RECURSION_TIME) {
        return glm::vec3{ 0, 0, 0 };
    }

    auto hitResult = getIntersection(ray);
    const Shape* collidedEntityPtr = hitResult.shape;

    if (hitResult.shape == nullptr) {
        return glm::vec3{ 0.4, 0.6, 0.8 }; // 背景色
    }

    bool enterEntity = collidedEntityPtr->rayInEntity(ray);
    if (enterEntity) {
        hitResult.normal = -hitResult.normal; // 法线反转
    }

    // 局部光照计算
    if (!enterEntity) {
        glm::vec3 shaderColor = shade(hitResult, ray);
        lightIntensity = collidedEntityPtr->getMaterial().kShade_ * shaderColor;
    }

    // 反射计算
    if (collidedEntityPtr->getMaterial().kReflect_ > FLOAT_EPS) {
        glm::vec3 reflectDirection = glm::reflect(ray.getDirection(), hitResult.normal);
        glm::vec3 reflectStart = hitResult.hitPoint + hitResult.normal * 0.001f; // 微小偏移
        glm::vec3 reflectEnd = reflectStart + reflectDirection * 100.0f; // 确保足够长
        Ray reflectRay(reflectStart, reflectEnd);
		std::cout << "reflectDirection" << reflectDirection << "lightIntensity" << lightIntensity << std::endl;
        lightIntensity += collidedEntityPtr->getMaterial().kReflect_ * traceRay(reflectRay, recursionTime + 1);
		std::cout << "lightIntensity" << lightIntensity << "recursionTime" << recursionTime << std::endl;
    }

    // 折射计算
    if (collidedEntityPtr->getMaterial().kRefract_ > FLOAT_EPS) {
        float currentIndex = 1.0f;
        float nextIndex = collidedEntityPtr->getMaterial().refractiveIndex_;
        if (enterEntity) std::swap(currentIndex, nextIndex);

        float eta = enterEntity ? (currentIndex / nextIndex) : (nextIndex / currentIndex);
        glm::vec3 refractDirection = glm::refract(ray.getDirection(), hitResult.normal, eta);

        glm::vec3 refractStart = hitResult.hitPoint - hitResult.normal * 0.001f; // 微小偏移
        glm::vec3 refractEnd = refractStart + refractDirection * 100.0f;
        Ray refractRay(refractStart, refractEnd);;
        lightIntensity += collidedEntityPtr->getMaterial().kRefract_ * traceRay(refractRay, recursionTime + 1);
    }

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

glm::vec3 Scene::shade(const HitResult& hitRes, const Ray& ray)
{
    glm::vec3 result(0.0f);
    for (auto pLight : _lights)
    {

        //glm::vec3 lightDir = pLight->getDirection(hitRes.hitPoint);
        //Ray shadowRay(hitRes.hitPoint , lightDir);
        //HitResult shadowHit = getIntersection(shadowRay);
        //if (shadowHit.shape == nullptr ) {
        //    // if(entity.getName() == "Plane")
        //    // {
        //    //     // std::cout <<  "fragPos: "<< fragPos << " ray.getDirection() : " << ray.getDirection() << std::endl;
        //    //     result += pLight->calLight(entity.getMaterial(), fragPos, entity.calNormal(fragPos), ray.getDirection());
        //    //     // std::cout << result << "normal: "<< entity.calNormal(fragPos)  << std::endl;

        //    // }
        //    result += pLight->calLight(hitRes.shape->getMaterial(), hitRes.hitPoint, hitRes.normal, ray.getDirection());
        //    // std::cout << result << "normal: "<< entity.calNormal(fragPos)  << std::endl;
        //    // std::cout <<  "fragPos: "<< fragPos << " ray.getDirection() : " << ray.getDirection() << std::endl;
        //}
        result += pLight->calLight(hitRes.shape->getMaterial(), hitRes.hitPoint, hitRes.normal, ray.getDirection());
    }
    // if(result.x > 0)
    // {
    //     std::cout << result  << std::endl;

    // }
    result = glm::clamp(result, 0.0f, 1.0f);  // 关键！防止溢出
    return result;
}

