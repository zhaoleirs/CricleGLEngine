/*
 * Map.cpp
 *
 *  Created on: 2013-4-24
 *      Author: zhaolei
 */

#include "Map.h"
#include "Common.h"
#include "GLES/glext.h"

Map::Map() :
		layer0(0), fulldraw(true) {
	// TODO Auto-generated constructor stub
	int p2w = power2(screenW);
	int p2h = power2(screenH);
	this->extraW = p2w - screenW;
	this->extraH = p2h - screenH;
	this->buffer = GLBuffer::create(p2w, p2h);
}

Map::~Map() {
	clear();
}
void Map::clear() {
	for (map<short, GLuint>::iterator it = this->images.begin();
			it != this->images.end(); it++) {
		delete Texture::shareTexture(it->second);
	}
	this->images.clear();

	this->fulldraw = true;
}
void Map::draw(int x, int y) {
	//画地图到缓存
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, this->buffer->frame);
	glClear (GL_DEPTH_BUFFER_BIT);
	//实际绘制
	int bx_draw;
	int by_draw;
	int bw_draw;
	int bh_draw;
	//第一层需要平铺到缓存上面
	if (this->fulldraw) {
		bx_draw = x;
		by_draw = y;
		bw_draw = x + screenW;
		bh_draw = y + screenH;
	}
	//第一层
	if (layer0 != 0)
		Texture::shareTexture(images[layer0])->rectRepeat(bx_draw, by_draw,
				bw_draw, bh_draw);
	for (int row = 0; row < bh_draw / Grid; ++row) {
		row += by_draw / Grid;
		for (int col = 0; col < bw_draw / Grid; ++col) {
			col += bx_draw / Grid;
			//第二层
			short id = layer1[row << 2 | col];
			LOG("__ID_LAYER_1:%d", id);
			//第三层
			id = layer2[row << 2 | col];
			LOG("__ID_LAYER_2:%d", id);
			//第四层
			MapLayer layer = layer3[row << 2 | col];
			LOG("__ID_LAYER_3:%d", id);
		}
	}

	//画缓存到屏幕
	this->buffer->texture->draw(x, y);
	//画精灵和动画
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, NULL);
	this->fulldraw = false;
}
Map* Map::build(unsigned char* data, int len) {
	static Map* map;
	//-----------------------
	//读取组件
	//-----------------------
	BytesReader *reader = new BytesReader(data, len);
	for (int i = 0; i < reader->readShort(); i++) {
		Component* component = new Component();
		component->id = reader->readShort();
		component->extraId = reader->readShort();
		component->col_span = reader->readByte();
		component->row_span = reader->readByte();
		unsigned char data_len = reader->readByte();
		component->data = new int[data_len];
		for (int i = 0; i < data_len; i++) {
			component->data[i] = reader->readInt();
		}
		map->layerObjects.insert(
				std::map<unsigned short, Component*>::value_type(component->id,
						component));
	}

	//-----------------------
	//读取地图大小
	//-----------------------
	map->mapCol = reader->readShort();
	map->mapRow = reader->readShort();

	//-----------------------
	//读取第一层
	//-----------------------
	map->layer0 = reader->readShort();

	//-----------------------
	//读取第二层
	//-----------------------
	short layer1_size = reader->readShort();
	for (int i = 0; i < layer1_size; i++) {
		MapLayer* layer = new MapLayer();
		int _id = reader->readInt();
		short col = _id & 0xff;
		_id = _id >> 8;
		short row = _id & 0xff;
		_id = (_id >> 8); //Flip
		layer->flip = (_id & 7);
		_id = (_id >> 3); //图片ID
		unsigned short key = row << 2 | col;
		map->layer1.insert(
				std::map<unsigned short, MapLayer*>::value_type(key, layer));
	}

	//-----------------------
	//读取第三层
	//-----------------------
	short layer2_size = reader->readShort();
	for (int i = 0; i < layer2_size; i++) {
		MapLayer* layer = new MapLayer();
		int _id = reader->readInt();
		short col = _id & 0xff;
		_id = _id >> 8;
		short row = _id & 0xff;
		_id = (_id >> 8); //Flip
		layer->flip = (_id & 7);
		_id = (_id >> 3); //图片ID
		unsigned short key = row << 2 | col;
		map->layer2.insert(
				std::map<unsigned short, MapLayer*>::value_type(key, layer));
	}
	//-----------------------
	//读取第四层
	//-----------------------
	short layer3_size = reader->readShort();
	for (int i = 0; i < layer3_size; i++) {
		MapLayer* layer = new MapLayer();
		short rcid = reader->readShort();
		int row = (rcid & 0xff);
		rcid = (rcid >> 8);
		int col = (rcid & 0xff);
		unsigned short key = row << 2 | col;

		int _id = reader->readInt();
		layer->type = (_id & 1);
		if (layer->type == 0) {
			_id = (_id >> 1);
			layer->flip = (_id & 7);
			layer->id = ((_id >> 3) << 1);
		} else {
			layer->id = _id;
		}
		map->layer3.insert(
				std::map<unsigned short, MapLayer*>::value_type(key, layer));

	}

	//-----------------------
	//读取出入口
	//-----------------------
	unsigned char door = reader->readByte();
	return map;
}

void Map::insertImage(short id, GLuint texture) {
	this->images.insert(map<short, GLuint>::value_type(id, texture));
}

