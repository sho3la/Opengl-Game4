#include "PointLight.h"
using namespace glm;

PointLight::PointLight(void):GameObject(NULL)
{
	color = vec3(1);
	intensity = 1;
}


PointLight::~PointLight(void)
{
}
