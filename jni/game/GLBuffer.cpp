/*
 * GLBuffer.cpp
 *
 *  Created on: 2013-4-11
 *      Author: zhaolei
 */

#include "GLBuffer.h"
#include <GLES/glext.h>

map<GLuint, GLBuffer*> GLBuffer::buffer;
GLBuffer::GLBuffer() {

}

GLBuffer::~GLBuffer() {
	glDeleteFramebuffersOES(1, &this->frame);
	glDeleteRenderbuffersOES(1, &this->render);
	delete texture;
}

GLBuffer* GLBuffer::create(int w, int h) {
	GLBuffer* glBuffer = new GLBuffer();
	glBuffer->load(w,h);
	glBuffer->texture=Texture::create();
	glBuffer->texture->bind(w,h);
	buffer.insert(map<GLuint, GLBuffer*>::value_type(glBuffer->frame, glBuffer));
	return glBuffer;
}

void GLBuffer::load(int w, int h) {
	this->width = w;
	this->height = h;
	glGenFramebuffersOES(1, &this->frame);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, this->frame);

	glGenRenderbuffersOES(1, &this->render);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, this->render);

	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, w,
			h);
	glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES,
			GL_TEXTURE_2D, this->texture->texture, 0);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES,
			GL_RENDERBUFFER_OES, this->render);
}

GLBuffer* GLBuffer::shareBuffer(GLuint frame) {
	map<GLuint,GLBuffer*>::iterator it= buffer.find(frame);
	if(it!=buffer.end()){
		return it->second;
	}
	return NULL;
}
