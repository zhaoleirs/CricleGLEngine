/*
 * GLBuffer.h
 *
 *  Created on: 2013-4-11
 *      Author: zhaolei
 */
#include "GLES/gl.h"
#include "Texture.h"
#include <map>
#ifndef GLBUFFER_H_
#define GLBUFFER_H_
using namespace std;
class GLBuffer {
private:
	static map<GLuint,GLBuffer*> buffer;
public:
	static GLBuffer* create(int w,int h);
	static GLBuffer* shareBuffer(GLuint frame);
	int width;
	int height;
	GLuint frame;
	GLuint render;
	Texture* texture;
	GLBuffer();
	void load(int w, int h);
	virtual ~GLBuffer();
};

#endif /* GLBUFFER_H_ */
