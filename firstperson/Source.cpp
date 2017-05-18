#include <iostream>
#include "OGL.h"
#include "Game.h"
using namespace std;
int main(){
	OGLPtr x = OGL::get();

	if(x->init())
		x->start(GamePtr(new Game()));

	return 0;
}