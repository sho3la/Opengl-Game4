#include "OGL.h"
#include <exception>
#include <iostream>
using namespace std;

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"assimp.lib")

OGLPtr OGL::_instance = nullptr;

OGL::OGL(int GLMAJOR /* = 2 */, int GLMINOR /* = 1 */){
	_versionMajor = GLMAJOR;
	_versionMinor = GLMINOR;
	_time = 0;
	_shutdown = false;
}
OGL::~OGL(){
	if(_window)
		glfwDestroyWindow(_window);

	glfwTerminate();
}

OGLPtr OGL::get(int GLMajor,int GLMinor){
	if(_instance == nullptr){
		_instance = OGLPtr(new OGL(GLMajor,GLMinor));
	}
	return _instance;
}

GLFWwindow* OGL::getwindow()
{
	return _window;
}

GamePtr OGL::getGame(){
	return OGL::get()->_game;
}

glm::vec2 OGL::getMousePosition(){
	return _mousePos;
}

void OGL::setMousePosition(float x, float y){
	glfwSetCursorPos(_window,x,y);
	_mousePos = glm::vec2(x,y);
}

bool OGL::init(unsigned int width /* = 800 */, unsigned int height /* = 600 */, std::string title /* = "Window" */){
	_width = width;
	_height = height;
	_title = title;
	try{
		initWindow();
		return true;
	}catch(exception e){
		cerr<<e.what()<<endl;
		return false;
	}
}

void OGL::initWindow(){
	if(!glfwInit())
		throw exception("Cannot initialise GLFW");

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _versionMajor); //OpenGL version 3.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _versionMinor); // 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); //If requesting an OpenGL version below 3.2, GLFW_OPENGL_ANY_PROFILE

	_window = glfwCreateWindow(_width,_height,_title.c_str(),NULL,NULL);
	if(_window == nullptr)
		throw exception("Cannot create GLFW Window");

	glfwMakeContextCurrent(_window);

	if(glewInit() != GLEW_OK)
		throw exception("Cannot initialise GLEW");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void OGL::start(GamePtr game){
	_game = game;
	glfwSetWindowSizeCallback(_window,&OGL::windowResized);

	glfwSetCursorPosCallback(_window,&OGL::mouseMotion);
	startLoop();
}
void OGL::startLoop(){
	while(!glfwWindowShouldClose(_window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		update();
		render();
		
		glfwSwapBuffers(_window);
		glfwPollEvents();
		//if shutdown then break the loop
		if(_shutdown)
			break;
	}
}

void OGL::windowResized(GLFWwindow* window, int width, int height)
{
	OGLPtr app = OGL::get();
	app->_width = width;
	app->_height = height;
	glViewport(0,0,width,height);
}


void OGL::mouseMotion(GLFWwindow* window,double x, double y)
{
	_instance->_game->handleMouseMotion(window,x,y);
}

void OGL::update()
{
	double currentTime = glfwGetTime();
	double deltaTime = (currentTime - _time)*1000;
	_time = currentTime;

	_game->update(deltaTime);
}

void OGL::render()
{
	_game->render();
}

glm::vec2 OGL::getWindowSize()
{
	return glm::vec2(_width,_height);
}

void OGL::shutdown(){
	_shutdown = true;
}