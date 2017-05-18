#pragma once
#include <glm.hpp>
#include <memory>
#include "GameObject.h"

class Cube{ 
public:
	GameObjectPtr _cube;
	Cube();
	~Cube();

	void walk(glm::vec3,float);
};
