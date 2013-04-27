/*
 * Texture.h
 *
 *  Created on: 2013-4-7
 *      Author: zhaolei
 */
#include <GLES/gl.h>
#include <map>
#ifndef TEXTURE_H_
#define TEXTURE_H_
using namespace std;
class Texture {
private:
	static map<GLuint,Texture*> buffer;
public:
	static Texture* create();
	static Texture* shareTexture(GLuint texture);
	GLuint width;
	GLuint height;
	GLuint texture;
	Texture();
	void load();
	void draw(int x,int y);
	void draw(int sx,int sy,int sw,int sh,int dx,int dy,int dw,int dh);
	void rectRepeat(int,int,int,int);
	virtual ~Texture();
	void bind(unsigned char* data,unsigned int w,unsigned int h,int channel);
	void bind(unsigned int w,unsigned int h);
};

#endif /* TEXTURE_H_ */
