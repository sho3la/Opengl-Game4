#pragma once
#include "GameObject.h"


class Bullet
{
public:
	Bullet(ShaderPtr Shader);
	~Bullet();


	void Setpos(glm::vec3 _pos);
	glm::vec3 getpos();
	void setDir(glm::vec3 direction);
	void Update();
	void draw(glm::mat4 VP);
	bool fired;
	bool Collide(glm::vec3 otherpos); 


private:
	GameObjectPtr obj;
	glm::vec3 pos;
	glm::vec3 dir;

	

};

