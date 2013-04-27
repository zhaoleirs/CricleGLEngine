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
	unsigned short *modules;//[x,y,w,h]Ϊ1��,idΪ(����/4)
	unsigned int *frames; // frames֡�����
	unsigned int *actions; // ���������,ÿ��������һϵ��֡����
public:
	static Ani* create(short id);
	static Ani* shareAni(short id);
	Ani();
	virtual ~Ani();
	static void loadAeests(const char* aniFile);
	void play(int index,bool loop);

};

#endif /* ANIUNIT_H_ */
