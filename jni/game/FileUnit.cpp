/*
 * FileUnit.cpp
 *
 *  Created on: 2013-4-18
 *      Author: zhaolei
 */

#include "FileUnit.h"
#include <stdio.h>
#include "zip_support/unzip.h"
#include "Common.h"
FileUnit::FileUnit() {
	// TODO Auto-generated constructor stub

}

FileUnit::~FileUnit() {
	// TODO Auto-generated destructor stub
}

AssetsFile* FileUnit::getAssets(const char* file) {
	unsigned char * pBuffer = NULL;
	unzFile pFile = NULL;
	pFile = unzOpen(Common::APK_PATH);
	unzOpenCurrentFile(pFile);
	int nRet = unzLocateFile(pFile, file, 1);
	char szFilePathA[260];
	unz_file_info FileInfo;
	nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA,
			sizeof(szFilePathA), NULL, 0, NULL, 0);

	nRet = unzOpenCurrentFile(pFile);

	pBuffer = new unsigned char[FileInfo.uncompressed_size];
	int nSize = 0;
	nSize = unzReadCurrentFile(pFile, pBuffer, FileInfo.uncompressed_size);
	uLong pSize = FileInfo.uncompressed_size;
	unzCloseCurrentFile(pFile);
	if (pFile) {
		unzClose(pFile);
	}
	AssetsFile* _file=new AssetsFile();
	_file->data=pBuffer;
	_file->nSize=pSize;
	return _file;
}

