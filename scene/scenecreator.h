#ifndef RAY_SCENE_CREATOR_H
#define RAY_SCENE_CREATOR_H

#include "light.h"
#include "ray.h"
#include "scene.h"
class SceneCreator 
{
public:
    static class SceneCreator* GetInstance()
    {
        static SceneCreator instance;
        return &instance;
    }
    Scene* CreateScene(SceneType type);
    Scene* CreateBasicScene();
    Scene* CreateBVHScene(){};
private:
    SceneCreator(){};
    virtual ~SceneCreator(){};
};


#endif