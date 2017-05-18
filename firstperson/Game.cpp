#include"Game.h"
#include "Geometry.h"
#include "OGL.h"
#include <glm.hpp>
#include <iostream>
#include "Helper.h"
using namespace std;
using namespace glm;

GLFWwindow* wind;


//init,,,
Game::Game() 
{
	auto windowpos = OGL::get()->getWindowSize();

	OGL::get()->setMousePosition(windowpos.x/2,windowpos.y/2);

	//_turnOnMouseCamera = false;

	wind = OGL::get()->getwindow();


	_Shader = std::make_shared<Shader>("Model.vertexshader", "Model.fragmentshader");


	auto _skyboxShader = std::make_shared<Shader>("Skybox.vertexshader", "Skybox.fragmentshader");

	sky.setShader(_skyboxShader);

	_ground = new Ground(20,20);

	_walls = Ground::loadFile("res/map.txt");


	
	enemylight.intensity = 1;
	enemylight.color = vec3(1,0,1);
	enemylight.position = vec3(0,3,0);

	playerLight.position = camera.getPosition();
	playerLight.intensity = 1;
	playerLight.color = vec3(1,1,1);





	enemyPosition = vec3(2.25,1.6,15.4);

	enemyPosTarget = enemyPosition;

	monsterRotate = vec3(-90,0,180);

	model_m = glm::translate(enemyPosition) * glm::rotate(-90.0f,vec3(1.0f,0.0f,0.0f)) *glm::rotate(180.0f,vec3(0.0f,0.0f,1.0f)) * glm::scale(vec3(0.025f,0.025f,0.025f));


	//load model file..
	mymodel.LoadModel("res/drfreak/drfreak.md2");

	//set curent animation to play..
	model_anim_state = mymodel.StartAnimation(animType_t::CROUCH_WALK);

	//load animation shader.
	animatedModelShader.LoadProgram();

	camera.setPosition(vec3(0.5,1.76,0.5));




	bul = new Bullet(_Shader);

	bulletLight.color = vec3(0,1,1);
	bulletLight.intensity = 1;
	bulletLight.position = vec3(50,50,0);  //very high point


}

Game::~Game()
{
	delete _ground;
	delete _walls;

	delete bul;
}


void Game::setupShaderUniforms(ShaderPtr shdr)
{
	shdr->setUniform("TxSampler",0);
	shdr->setUniform("NrmSampler",1);

	shdr->setUniform("ViewMatrix",camera.getViewMatrix());
	//do the lights here
	shdr->setUniform("eyePos",camera.getPosition());

	shdr->setUniform("playerLightPosition",playerLight.position);
	shdr->setUniform("playerLightColor",playerLight.color);
	shdr->setUniform("playerLightIntensity",playerLight.intensity);

	shdr->setUniform("monsterLightPosition",enemylight.position);
	shdr->setUniform("monsterLightColor",enemylight.color);
	shdr->setUniform("monsterLightIntensity",enemylight.intensity);


	shdr->setUniform("BulletLightPosition",bulletLight.position);
	shdr->setUniform("BulletLightColor",bulletLight.color);
	shdr->setUniform("BulletLightIntensity",bulletLight.intensity);

}


void Game::update(double delta)
{
	cameraMouseMovement(0,0);



	//moving the light with the camera.
	enemylight.position = enemyPosition;
	playerLight.position = camera.getPosition();

	if(bul->fired)
		bulletLight.position = bul->getpos();



	mymodel.UpdateAnimation(&model_anim_state,delta / 1000);

	enemyMovment();

	auto mmx = glm::rotate(monsterRotate.x,vec3(1,0,0));
	auto mmy = glm::rotate(monsterRotate.y,vec3(0,1,0));
	auto mmz = glm::rotate(monsterRotate.z,vec3(0,0,1));
	auto mmr = mmx*mmy*mmz;
	model_m = glm::translate(enemyPosition) * mmr * glm::scale(vec3(0.025f,0.025f,0.025f));


	if( glfwGetKey(wind, GLFW_KEY_W))
	{
		camera.Walk(0.1);

		if(_walls->Collide(camera.getPosition()))
			camera.Walk(-0.1);
	}
	if( glfwGetKey(wind, GLFW_KEY_S))
	{
		camera.Walk(-0.1);
		if(_walls->Collide(camera.getPosition()))
			camera.Walk(0.1);
	}

	if( glfwGetKey(wind, GLFW_KEY_A))
	{
		camera.Strafe(-0.1);
		if(_walls->Collide(camera.getPosition()))
			camera.Strafe(0.1);
	}

	if( glfwGetKey(wind, GLFW_KEY_D))
	{
		camera.Strafe(0.1);
		if(_walls->Collide(camera.getPosition()))
			camera.Strafe(-0.1);
	}

	if( glfwGetKey(wind, GLFW_KEY_ESCAPE))
	{
		OGL::get()->shutdown();
	}



	if( glfwGetKey(wind, GLFW_KEY_SPACE))
	{

		bul->Setpos(glm::vec3(camera.getPosition().x,1.6,camera.getPosition().z));
		bul->setDir(camera.getLookDirection());

		bul->fired = true;
	}


	// prevent camera to move on y axis  just xz plane..
	if(camera.getPosition().y > 1.76 || camera.getPosition().y < 1.76 )
	{
		camera.setPosition(glm::vec3(camera.getPosition().x, 1.76 , camera.getPosition().z));
	}


	if(bul->Collide(enemyPosition) && bul->fired)
	{
		system("cls");
		printf("\n");
		printf("  YOU WIN ^____^ \n");
		printf("\n");
		OGL::get()->shutdown();
	}

	bul->Update();

	camera.updateViewMatrix();

}

void Game::render()
{

	glm::mat4 VP = camera.getProjectionMatrix()*camera.getViewMatrix();

	sky.draw(VP);

	_ground->draw(VP);

	_walls->draw(VP);

	bul->draw(VP);


	animatedModelShader.UseProgram();
	animatedModelShader.BindVPMatrix(&VP[0][0]); 

	animatedModelShader.BindModelMatrix(&model_m[0][0]); 

	mymodel.RenderModel(&model_anim_state,&animatedModelShader);

}

void Game::enemyMovment()
{
	int rnd = rand()%4;

	vec3 bpos = enemyPosition;
	if(enemyPosTarget == enemyPosition)
	{
		if(rnd == 1)
		{
			enemyPosTarget.x += 1;
			enemyDirection = Direction::NORTH;
			monsterRotate.z = 0;
		}else if(rnd==2)
		{
			enemyPosTarget.x -= 1;
			enemyDirection = Direction::SOUTH;
			monsterRotate.z = 180;
		}else if(rnd==3)
		{
			enemyPosTarget.z += 1;
			enemyDirection = Direction::EAST;
			monsterRotate.z = 270;
		}else if(rnd==0){
			enemyPosTarget.z -= 1;
			enemyDirection = Direction::WEST;
			monsterRotate.z = 90;
		}
		if(_walls->Collide(enemyPosTarget))
			enemyPosTarget = enemyPosition;
		//bpos = monsterPosTarget;
	}else{
		if(enemyDirection == Direction::NORTH){
			bpos.x += 0.05;
		}else if(enemyDirection == Direction::SOUTH){
			bpos.x -= 0.05;
		}else if(enemyDirection == Direction::EAST){
			bpos.z += 0.05;
		}else if(enemyDirection == Direction::WEST){
			bpos.z -= 0.05;
		}
	}

	enemyPosition = bpos;
	if(_walls->Collide(bpos)){
		enemyPosTarget = enemyPosition;

		if(enemyDirection == Direction::NORTH){
			enemyPosTarget.x -= 0.5;
		}else if(enemyDirection == Direction::SOUTH){
			enemyPosTarget.x += 0.5;
		}else if(enemyDirection == Direction::EAST){
			enemyPosTarget.z -= 0.5;
		}else if(enemyDirection == Direction::WEST){
			enemyPosTarget.z += 0.5;
		}
		enemyPosition = enemyPosTarget;
	}else{

	}
}


void Game::handleMouseMotion(GLFWwindow* window,int x, int y)
{
	mousePosition = vec2(x,y);
}

void Game::cameraMouseMovement(int x, int y)
{
	auto windowSize = OGL::get()->getWindowSize();


	if (mousePosition.x != windowSize.x/2 || mousePosition.y != windowSize.y/2)
	{
		double mouseSpeed = 0.005; //it is just there to speed up or slow down the movements.
		double movedDistanceX;
		double movedDistanceY;


		movedDistanceX = double(windowSize.x/2 - mousePosition.x)*mouseSpeed;
		movedDistanceY = double(windowSize.y/2 - mousePosition.y)*mouseSpeed;


		camera.Yaw(movedDistanceX);
		camera.Pitch(movedDistanceY);
		camera.updateViewMatrix();

		mousePosition.x = windowSize.x/2;
		mousePosition.y = windowSize.y/2;
		OGL::get()->setMousePosition(mousePosition.x,mousePosition.y);
	}
}
