#include "light.h"

DirLight::DirLight(
	glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular,
	glm::vec3 direction)
	: _ambient(ambient),
	_diffuse(diffuse),
	_specular(specular),
	_direction(direction)
{

}

glm::vec3 DirLight::calLight(
	const Material& material,
	const glm::vec3& fragPos,
	const glm::vec3& norm,
	const glm::vec3& viewDir) const 
{
	glm::vec3 ambient = _ambient;

	glm::vec3 lightDir = normalize(-_direction);
	float diff = std::max(dot(norm, lightDir), 0.0f);
	glm::vec3 diffuse = diff * _diffuse * material.diffuse_;

	glm::vec3 middle = glm::normalize(-viewDir + lightDir);
	float spec = glm::pow(std::max(glm::dot(middle, norm), 0.0f), material.shininess_);
	glm::vec3 specular = _specular * spec * material.specular_;
	// std::cout << "ambient: " << ambient << "diffuse: " << diffuse << "specular: " << specular << "(ambient + diffuse + specular)" <<(ambient + diffuse + specular) << std::endl;

	return (ambient + specular);
}


PointLight::PointLight(
	glm::vec3 pos,
	glm::vec3 lightStrength)
	: pos_(pos),
	lightStrength_(lightStrength)
{

}

glm::vec3 PointLight::calLight(
	const Material& material,
	const glm::vec3& fragPos,
	const glm::vec3& norm,
	const glm::vec3& viewDir) const
{
	glm::vec3 ambient = lightStrength_ * glm::vec3(1.0f);

	// glm::vec3 lightDir = normalize(pos_ - fragPos);
	glm::vec3 lightDir = normalize(-glm::vec3(-0.5f, -1.0f, -1.0f));
	float diff = std::max(dot(norm, lightDir), 0.0f);
	glm::vec3 diffuse = diff * material.diffuse_ ;

	glm::vec3 middle = glm::normalize(-viewDir + lightDir);
	float spec = glm::pow(std::max(glm::dot(middle, norm), 0.0f), material.shininess_);
	glm::vec3 specular = spec * material.specular_ ;
	// std::cout << "ambient: " << ambient << "diffuse: " << diffuse << "specular: " << specular << "(ambient + diffuse + specular)" <<(ambient + diffuse + specular) << std::endl;

	return (ambient+ diffuse); ;
}