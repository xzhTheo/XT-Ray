
#include "scenecreator.h"
#include "scene.h"
Scene* SceneCreator::CreateScene(SceneType type)
{
    switch (type)
    {
    case SceneType::RAY_TRACING_BASIC_SCENE:
        return CreateBasicScene();
    //case SceneType::RAY_TRACING_BVH_SCENE:
    //    return CreateBVHScene();
    default:
        return CreateBasicScene();
    }
    return CreateBasicScene();
}


Scene* SceneCreator::CreateBasicScene()
{
    Scene* scene = new Scene();
	//scene->addLight(new DirLight(
	// 	glm::vec3(0.5f, 0.5f, 0.5f),
	// 	glm::vec3(0.5f, 0.5f, 0.5f),
	// 	glm::vec3(0.5f, 0.5f, 0.5f),
	// 	glm::vec3(-0.5f, -1.0f, -1.0f)
	// ));
	scene->addLight(new PointLight(
		glm::vec3(5.0f, 7.2f, 3.2f),
		glm::vec3(0.2f, 0.2f, 0.2f)
	));
	Material planeMaterial;
	planeMaterial.kShade_ = 0.7f;
	planeMaterial.kReflect_ = 222.8f;
	planeMaterial.kRefract_ = 222.8f;

	planeMaterial.diffuse_ = glm::vec3(1.0f, 1.0f, 1.0f ) ;
	planeMaterial.specular_ = glm::vec3(0.1f, 0.6f, 1.0f);
	planeMaterial.shininess_ = 32.0f;
	Plane* plane = new Plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	plane->setMaterial(planeMaterial);
	scene->addEntity(plane);

	Material ballMaterial;
	ballMaterial.kShade_ = 0.6f;
	ballMaterial.kReflect_ = 222.8f;
	ballMaterial.kRefract_ = 222.0f;
	ballMaterial.refractiveIndex_ = 1.5f;
	ballMaterial.diffuse_ = { 1.0f, 1.0f, 1.0f  };
	ballMaterial.specular_ = {1.0f, 1.0f, 1.0f  };
	ballMaterial.shininess_ = 32.0f;

	auto ball = new Sphere(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
	ball->setMaterial(ballMaterial);

    auto ball2 = new Sphere(glm::vec3(-2.5f, 1.0f, 0.0f), 1.0f);
    ball2->setMaterial(ballMaterial);

    auto ball3 = new Sphere(glm::vec3(0.0f, 3.0f, 0.0f), 1.0f);
    ball3->setMaterial(ballMaterial);

	scene->addEntity(ball);
    scene->addEntity(ball2);
    scene->addEntity(ball3);
    return scene;
}