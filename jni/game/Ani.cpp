/*
 * Ani.cpp
 *
 *  Created on: 2013-4-18
 *      Author: zhaolei
 */

#include "Ani.h"
#include <stdio.h>
#include "Common.h"
#include "FileUnit.h"
#include "BytesReader.h"
#include "PngUtil.h"
map<short,Ani*> Ani::buffer;
Ani::Ani() :
		pelay(0) ,index(0){
}

Ani::~Ani() {
	delete[] modules;
	delete[] frames;
}

Ani* Ani::create(short id) {
	Ani* ani=new Ani();
	ani->ani = id; // id 暂时没用
	LOG("create ani:%d",id);
	buffer.insert(map<short,Ani*>::value_type(id,ani));
	return ani;
}

Ani* Ani::shareAni(short id) {
	map<short,Ani*>::iterator it= buffer.find(id);
	if(it!=buffer.end()){
		return it->second;
	}
	return NULL;
}

void Ani::loadAeests(const char* aniFile) {

	AssetsFile* _file = FileUnit::getAssets(aniFile);
	BytesReader* reader = new BytesReader(_file->data, _file->nSize);
//	reader->skip(3);
	int pngLength = reader->readInt();
	unsigned char* png_data = reader->readBytes(pngLength);
	GLPNG* png = PngUtil::read(png_data, pngLength);
	Texture* texture = Texture::create();
	texture->bind(png->data, png->width, png->height, png->channel);
	int loop = reader->readByte();
	for (int i = 0; i < loop; i++) {
		short id = reader->readShort();
		if (Ani::shareAni(id))
			continue;
		Ani* ani = Ani::create(id);
		ani->texture = texture;
		int length = reader->readInt();
		ani->frame_size = reader->readShort();
		ani->modules = new unsigned short[ani->frame_size * 4];
		for (int i = 0; i < ani->frame_size; i++) {
			// module_x
			ani->modules[i * 4] = reader->readShort(); // dis.readShort();//
			// module_y
			ani->modules[i * 4 + 1] = reader->readShort(); // dis.readShort();//
			// module_w
			ani->modules[i * 4 + 2] = reader->readShort(); // dis.readShort();//
			// module_h
			ani->modules[i * 4 + 3] = reader->readShort(); // dis.readShort();//
		}
		// 帧
		short _num = reader->readShort();
		ani->frames = new unsigned int[_num * 2];
		for (int i = 0; i < _num; i++) {
			unsigned int _size = reader->readByte();
			unsigned int *_frame = new unsigned int[_size * 4];
			ani->frames[i * 2] = _size;
			ani->frames[i * 2 + 1] = (unsigned int) _frame;

			for (int j = 0; j < _size; j++) {
				_frame[j * 4] = reader->readShort(); //id
				_frame[j * 4 + 1] = reader->readShort(); //x
				_frame[j * 4 + 2] = reader->readShort(); //y
				_frame[j * 4 + 3] = reader->readByte(); //flip
			}
		}

		if (reader->end())
			return;
		unsigned int action_size = reader->readByte();
		ani->actions = new unsigned int[action_size * 2];
		for (int i = 0; i < action_size; ++i) {
			unsigned int _size = reader->readByte();
			unsigned int *_action = new unsigned int[_size * 2];
			ani->actions[i * 2] = _size;
			ani->actions[i * 2 + 1] = (unsigned int) _action;
			for (int j = 0; j < _size; ++j) {
				_action[j * 2] = reader->readShort();
				_action[j * 2 + 1] = reader->readByte();
			}
		}
	}

	delete png;
	delete _file;
}

void Ani::play(int index,bool loop) {

	texture->load();
	unsigned int aSize = this->actions[index*2];

	unsigned int * _action = (unsigned int *) this->actions[index*2+1];
	short frameId = _action[this->index*2]; //帧ID
	//帧ID
	unsigned int size = this->frames[frameId*2];
	unsigned int * _frame = (unsigned int *) this->frames[frameId*2 + 1];
	for (int i = 0; i < size; ++i) {
//		short id=*();
		short id = _frame[i * 4]; // = reader->readShort();
		short x = _frame[i * 4 + 1] + 200; // = reader->readShort();
		short y = _frame[i * 4 + 2] + 200; // = reader->readShort();
		unsigned char flip = _frame[i * 4 + 3]; // = reader->readByte();
		LOG("play:%d",flip);
		if (id > 0)
			texture->draw(this->modules[0 + 4 * id], this->modules[1 + 4 * id],
					this->modules[2 + 4 * id], this->modules[3 + 4 * id], x,
					screenH - y - this->modules[3 + 4 * id],
					this->modules[2 + 4 * id], this->modules[3 + 4 * id]);
	}

	long time = getCurrentTime();
	if (time - pelay > FPS) {
		//改变FPS
		pelay = time;
		this->index++;
		if(this->index>=aSize){
			if(loop){
				this->index=0;
			}else{
				this->index--;
			}
		}
	}
}
