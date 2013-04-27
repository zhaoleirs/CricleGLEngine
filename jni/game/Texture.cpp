/*
 * Texture.cpp
 *
 *  Created on: 2013-4-7
 *      Author: zhaolei
 */

#include "Texture.h"
#include <stdio.h>
#include <android/log.h>
#include <GLES/glext.h>
#include "Common.h"

map<GLuint, Texture*> Texture::buffer;
Texture::Texture() {
	glGenTextures(1, &texture);
}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
	buffer.erase(texture);
}
void Texture::load() {
	glBindTexture(GL_TEXTURE_2D, texture);
}

Texture* Texture::create() {
	Texture* texture = new Texture();
	buffer.insert(map<GLuint, Texture*>::value_type(texture->texture, texture));
	return texture;
}

Texture* Texture::shareTexture(GLuint texture) {
	map<GLuint, Texture*>::iterator it = buffer.find(texture);
	if (it != buffer.end()) {
		return it->second;
	}
	return NULL;
}
void Texture::draw(int x, int y) {
	draw(0, 0, width, height, x, y, width, height);
}

void Texture::draw(int sx, int sy, int sw, int sh, int dx, int dy, int dw,
		int dh) {
	glBindTexture(GL_TEXTURE_2D, texture);
	if (sw == 0)
		sw = this->width;
	if (sh == 0)
		sh = this->height;
	if (dw == 0)
		dw = sw;
	if (dh == 0)
		dh = sh;
	GLint cropRect[4] = { sx, (sy + sh), sw, -sh };
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, cropRect);
	glDrawTexiOES(dx, dy, 0, dw, dh);
}
void Texture::rectRepeat(int x, int y, int w, int h) {
	this->load();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	GLint cropRect[4] = { 0, height, width, -height };
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, cropRect);
	glDrawTexiOES(x, y, 0, w, h);
}
void Texture::bind(unsigned int w, unsigned int h) {
	glBindTexture(GL_TEXTURE_2D, texture);
	this->width = w;
	this->height = h;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
			NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

}
void Texture::bind(unsigned char* data, unsigned int w, unsigned int h,
		int channel) {
	glBindTexture(GL_TEXTURE_2D, texture);
	this->width = w;
	this->height = h;
	int pw = power2(w);
	int ph = power2(h);
	int type = GL_RGB;
	if (channel == 4)
		type = GL_RGBA;
	if (w != pw && h != ph) {
		glTexImage2D(GL_TEXTURE_2D, 0, type, pw, ph, 0, type, GL_UNSIGNED_BYTE,
				NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, type, GL_UNSIGNED_BYTE,
				data);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, type, pw, ph, 0, type, GL_UNSIGNED_BYTE,
				data);
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

