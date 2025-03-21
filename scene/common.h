
#ifndef RAY_COMMON_H
#define RAY_COMMON_H
#include <iostream>
#include <glm/glm.hpp>
// 重载输出流操作符以打印 glm::mat4
inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

inline bool isVec3Zero(const glm::vec3& vec) {
    return vec.x == 0.0f && vec.y == 0.0f && vec.z == 0.0f;
}

enum class SceneType
{
    RAY_TRACING_BASIC_SCENE                  = 1,
    RAY_TRACING_BVH_SCENE                    = 2,
};

struct Material
{
	glm::vec3 ambient_;
	glm::vec3 diffuse_;
	glm::vec3 specular_;
	glm::vec3 shininess_;
	float kShade_;
	float kReflect_;
	float kRefract_;
	float refractiveIndex_;
	glm::vec3 kColor_;
};

#endif