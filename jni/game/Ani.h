/*
 * AniUnit.h
 *
 *  Created on: 2013-4-18
 *      Author: zhaolei
 */
#include "Texture.h"
#include <map>
#ifndef ANIUNIT_H_
#define ANIUNIT_H_
using namespace std;

class Ani {
private:
	static map<short,Ani*> buffer;
	short ani;
	Texture* texture;
	long pelay;
	unsigned int index;
	unsigned short frame_size;
	unsigned short *modules;//[x,y,w,h]为1个,id为(索引/4)
	unsigned int *frames; // frames帧的组合
	unsigned int *actions; // 动作的组合,每个动作由一系列帧构成
public:
	static Ani* create(short id);
	static Ani* shareAni(short id);
	Ani();
	virtual ~Ani();
	static void loadAeests(const char* aniFile);
	void play(int index,bool loop);

};

#endif /* ANIUNIT_H_ */
