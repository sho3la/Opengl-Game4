#include "GameObject.h"
#include "Material.h"
#include "Shader.h"

void GameObject::update(double delta)
{

}
void GameObject::draw(glm::mat4 vp,int is_Instancing)
{

	mesh->getMaterial()->shader->setUniform("MVP",vp*getModel());


	mesh->getMaterial()->shader->setUniform("ModelMatrix",getModel());

	mesh->draw(is_Instancing);

}