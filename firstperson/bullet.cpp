#include "bullet.h"
#include "Helper.h"
#include <glm.hpp>


Bullet::Bullet(ShaderPtr Shader) : pos(glm::vec3(0.5,1.76,0.5))
{
	auto mesh = Helper::loadMesh("res/cube/cube.obj");
	mesh->setShader(Shader);
	obj = std::make_shared<GameObject>(mesh); 
	obj->position = pos;
	obj->scale = glm::vec3(0.08,0.08,0.08);


	fired = false;
}

Bullet::~Bullet()
{
}


bool Bullet::Collide(glm::vec3 otherpos)
{
	if(glm::distance(pos,otherpos) < 0.5 && fired)
	{
		return true;
	}

	return false;
}



void Bullet::Update()
{
	//to fire bullet from camera position 
	//you will need direction of view and this made by camera.direction 
	// secondly update position  of bullet by adding vector of direction to its position ...
	if(fired)
	{
		pos.x -= (dir.x) / 10;
		pos.z -= (dir.z) / 10;
		obj->position = pos;
	}
}


void Bullet::draw(glm::mat4 VP)
{
	if (fired)
		obj->draw(VP);
}

void Bullet::Setpos(glm::vec3 _pos)
{
	pos = _pos;
}

glm::vec3 Bullet::getpos()
{
	return pos;
}


void Bullet::setDir(glm::vec3 direction)
{
	dir = direction;
}