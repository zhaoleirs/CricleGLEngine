/*
 * PngUtil.h
 *
 *  Created on: 2013-4-7
 *      Author: zhaolei
 */
#include "Texture.h"
#include "stdio.h"
#ifndef PNGUTIL_H_
#define PNGUTIL_H_
#define CC_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
    ((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
    ((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
    ((unsigned)(unsigned char)(va) << 24))

typedef struct _png {
	~_png(){
		delete[] data;
		data=NULL;
	}
	unsigned char* data;
	int width;
	int height;
	int channel;
} GLPNG;
class PngUtil {
public:
	static GLPNG* readAssets(const char* file);
	static GLPNG* read(const char* file);
	static GLPNG* read(unsigned char* data,int nDatalen);
};

#endif /* PNGUTIL_H_ */
