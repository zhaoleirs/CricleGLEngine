/*
 * Map.h
 *
 *  Created on: 2013-4-24
 *      Author: zhaolei
 */
#include <map>
#ifndef MAP_H_
#define MAP_H_
typedef struct _MapLayer{
	short id;
	unsigned char flip;
	unsigned char type;
}MapLayer;

typedef struct _Component{
	short id;
	short extraId;
	unsigned char col_span;
	unsigned char row_span;
	int* data;

}Component;

using namespace std;

class Map {
private:
	bool fulldraw;//第一次需要绘制全屏
	short layer0;//地面
	map<unsigned short,MapLayer*> layer1;//第一层
	map<unsigned short,MapLayer*> layer2;//第二层
	map<unsigned short,MapLayer*> layer3;//第三层
	map<unsigned short,Component*> layerObjects;//第三层
	map<short,GLuint> images;//图片ID对应纹理

	short mapCol;
	short mapRow;

	GLBuffer* buffer;//缓存
	int extraW;
	int extraH;

public:
	Map();
	virtual ~Map();
	void insertImage(short,GLuint);
	static Map* build(unsigned char* data,int len);
	void clear();
	void draw(int x,int y);
};

#endif /* MAP_H_ */
