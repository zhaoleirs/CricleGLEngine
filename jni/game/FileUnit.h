/*
 * FileUnit.h
 *
 *  Created on: 2013-4-18
 *      Author: zhaolei
 */

#ifndef FILEUNIT_H_
#define FILEUNIT_H_
typedef struct __File{
	~__File(){
		delete[] data;
	}
	int nSize;
	unsigned char* data;
} AssetsFile;
class FileUnit {
public:
	FileUnit();
	virtual ~FileUnit();
	static AssetsFile* getAssets(const char* path);
};

#endif /* FILEUNIT_H_ */
