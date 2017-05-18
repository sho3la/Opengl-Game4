#pragma once
#include<memory>
#include "GameObject.h"
#include "Shader.h"
#include<GLFW/glfw3.h>
#include"EulerCamera.h"
#include <glm.hpp>
#include <map>
#include <string>
#include "Skybox.h"
#include "PointLight.h"
#include "Ground.h"
#include "md2model.h"

#include "bullet.h"

class Game{
private:
	enum Direction{SOUTH,NORTH,EAST,WEST};

	ShaderPtr _Shader;

	Skybox sky;

	Ground* _ground;


	Ground* _walls;


	CMD2Model mymodel;
	animState_t model_anim_state;
	KeyFrameAnimationShader animatedModelShader;


	glm::mat4 model_m;

	glm::vec3 enemyPosition;
	Direction enemyDirection;
	glm::vec3 enemyPosTarget;
	glm::vec3 monsterRotate;


	Bullet *bul;


	PointLight playerLight, enemylight , bulletLight;


	glm::vec2 mousePosition;
	//bool _turnOnMouseCamera;
public:
	EulerCamera camera;
	Game();
	~Game();

	void enemyMovment();

	void setupShaderUniforms(ShaderPtr shdr);


	void update(double delta);
	void render();


	void cameraMouseMovement(int x, int y);


	//for handling mouse motion
	void handleMouseMotion(GLFWwindow* window,int x, int y);


};
typedef std::shared_ptr<Game> GamePtr;