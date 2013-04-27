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
	bool fulldraw;//��һ����Ҫ����ȫ��
	short layer0;//����
	map<unsigned short,MapLayer*> layer1;//��һ��
	map<unsigned short,MapLayer*> layer2;//�ڶ���
	map<unsigned short,MapLayer*> layer3;//������
	map<unsigned short,Component*> layerObjects;//������
	map<short,GLuint> images;//ͼƬID��Ӧ����

	short mapCol;
	short mapRow;

	GLBuffer* buffer;//����
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
