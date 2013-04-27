/*
 * BytesReader.h
 *
 *  Created on: 2013-4-18
 *      Author: zhaolei
 */

#ifndef BYTESREADER_H_
#define BYTESREADER_H_

class BytesReader {
private:
	unsigned int length;
	unsigned char* data;
	int index;
public:
	BytesReader(unsigned char*,unsigned int);
	virtual ~BytesReader();
	void skip(int length);
	unsigned char readByte();
	short readShort();
	int readInt();
	unsigned char* readBytes(int length);
	bool end();
};

#endif /* BYTESREADER_H_ */
