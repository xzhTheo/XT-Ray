
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
	planeMaterial.kReflect_ = 0.5f;
	planeMaterial.kRefract_ = 0.3f;

	planeMaterial.diffuse_ = glm::vec3(1.0f, 1.0f, 1.0f ) ;
	planeMaterial.specular_ = glm::vec3(0.1f, 0.6f, 1.0f);
	planeMaterial.shininess_ = 32.0f;
	Plane* plane = new Plane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	plane->setMaterial(planeMaterial);
	scene->addEntity(plane);

	Material ballMaterial;
	ballMaterial.kShade_ = 0.6f;
	ballMaterial.kReflect_ = 0.5f;
	ballMaterial.kRefract_ = 0.3f;
	ballMaterial.refractiveIndex_ = 1.5f;
	ballMaterial.diffuse_ = { 1.0f, 1.0f, 1.0f  };
	ballMaterial.specular_ = {1.0f, 1.0f, 1.0f  };
	ballMaterial.shininess_ = 32.0f;

	Material ballMaterial1;
	ballMaterial1.kShade_ = 0.6f;
	ballMaterial1.kReflect_ = 0.5f;
	ballMaterial1.kRefract_ = 0.3f;
	ballMaterial1.refractiveIndex_ = 1.5f;
	ballMaterial1.diffuse_ = { 0.3f, 0.6f, 0.9f  };
	ballMaterial1.specular_ = {0.3f, 0.6f, 0.9f   };
	ballMaterial1.shininess_ = 32.0f;


	Material ballMaterial2;
	ballMaterial2.kShade_ = 0.6f;
	ballMaterial2.kReflect_ = 0.5f;
	ballMaterial2.kRefract_ = 0.3f;
	ballMaterial2.refractiveIndex_ = 1.5f;
	ballMaterial2.diffuse_ = { 0.2f, 0.5f, 0.7f  };
	ballMaterial2.specular_ = {0.5f, 0.6f, 0.4f  };
	ballMaterial2.shininess_ = 32.0f;

	auto ball = new Sphere(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
	ball->setMaterial(ballMaterial);

    auto ball2 = new Sphere(glm::vec3(-2.5f, 1.0f, 0.0f), 1.0f);
    ball2->setMaterial(ballMaterial1);

    auto ball3 = new Sphere(glm::vec3(0.0f, 3.0f, 0.0f), 1.0f);
    ball3->setMaterial(ballMaterial2);

	scene->addEntity(ball);
    scene->addEntity(ball2);
    scene->addEntity(ball3);
    return scene;
}