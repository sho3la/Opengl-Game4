#pragma once
#include <glm.hpp>
#include "GameObject.h"

class PointLight : public GameObject
{
public:
	glm::vec3 color;
	float intensity;

	PointLight(void);
	~PointLight(void);

};

