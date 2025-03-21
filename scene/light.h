
#ifndef RAY_MATERIAL_H
#define RAY_MATERIAL_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "common.h"
#include <algorithm>
#include <functional>
class Light
{
public:
	virtual glm::vec3 calLight(
		const Material& material,
		const glm::vec3& fragPos,
		const glm::vec3& norm,
		const glm::vec3& viewDir) const = 0;
private:
};


//平行光
class DirLight : public Light
{
public:
	DirLight(
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		glm::vec3 direction);
	glm::vec3 calLight(
		const Material& material,
		const glm::vec3& fragPos,
		const glm::vec3& norm,
		const glm::vec3& viewDir) const;
private:
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	glm::vec3 _direction;
};

//点光源
class PointLight : public Light
{
public:
	PointLight(
		glm::vec3 pos,
		glm::vec3 lightStrength);
	glm::vec3 calLight(
		const Material& material,
		const glm::vec3& fragPos,
		const glm::vec3& norm,
		const glm::vec3& viewDir) const;
private:
	glm::vec3 pos_;
	glm::vec3 lightStrength_;
};


#endif