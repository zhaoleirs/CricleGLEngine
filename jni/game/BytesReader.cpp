/*
 * BytesReader.cpp
 *
 *  Created on: 2013-4-18
 *      Author: zhaolei
 */

#include "BytesReader.h"
#include "Common.h"
#include <stdlib.h>
BytesReader::BytesReader(unsigned char* data,unsigned int length):index(0),length(length) {
	this->data=data;
}

BytesReader::~BytesReader() {
}
bool BytesReader::end(){
	return index>=this->length;
}
void BytesReader::skip(int length){
	index+=length;
}

unsigned char BytesReader::readByte(){
	index+=1;
	return data[index-1];
}
short BytesReader::readShort(){
	index+=2;
	return data[index-2]<< 8 | data[index-1] ;

}
int BytesReader::readInt(){
	index+=4;
	return data[index-4]<< 24 |data[index-3] << 16|data[index-2] << 8|data[index-1];
}

unsigned char* BytesReader::readBytes(int length){
	unsigned char* value=new unsigned char[length];
	memcpy(value,&data[index],length);
	index+=length;
	return value;
}
