#ifndef GraphicsDevice_h__
#define GraphicsDevice_h__

#include <gl/glew.h>
#include "shader.hpp"


class ShaderProgram
{
	public:
	GLuint programID;

	GLuint modelMatrixID;
	GLuint vpMatrixID;

	ShaderProgram();
	~ShaderProgram();

	void BindModelMatrix(GLfloat* value);
	void BindVPMatrix(GLfloat* value);
	
	//calls glUseProgram.
	void UseProgram();
	void CleanUp();
};

#endif // GraphicsDevice_h__