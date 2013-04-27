/*
 * PngUtil.cpp
 *
 *  Created on: 2013-4-7
 *      Author: zhaolei
 */
#include "../libpng/png.h"
#include "../libpng/pngconf.h"
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include "PngUtil.h"
#include "FileUnit.h"

typedef struct {
	unsigned char* data;
	int size;
	int offset;
} tImageSource;

static void pngReadCallback(png_structp png_ptr, png_bytep data,
		png_size_t length) {
	tImageSource* isource = (tImageSource*) png_get_io_ptr(png_ptr);

	if ((int) (isource->offset + length) <= isource->size) {
		memcpy(data, isource->data + isource->offset, length);
		isource->offset += length;
	} else {
		png_error(png_ptr, "pngReaderCallback failed");
	}
}

GLPNG* PngUtil::read(const char* file) {
	FILE *fp = fopen(file, "rb");
	if (!fp) {
		return (NULL);
	}
	unsigned int number = 8;
	png_byte header[number];
	//ºÏ≤È «∑Ò «PNG
	fread(header, 1, number, fp);
	int is_png = !png_sig_cmp(header, 0, number);
	if (!is_png) {
		fclose(fp);
		return (NULL);
	}
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
			(png_voidp) NULL, NULL, NULL);
	if (!png_ptr) {
		fclose(fp);
		return (NULL);
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		fclose(fp);
		return (NULL);
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return (NULL);
	}

	//init png reading
	png_init_io(png_ptr, fp);

	//let libpng know you already read the f bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image datairst 8
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 width, height;

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	//CCLOG("color type %u", color_type);

	// force palette images to be expanded to 24-bit RGB
	// it may include alpha channel
	if (color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
	}
	// low-bit-depth grayscale images are to be expanded to 8 bits
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	}
	// expand any tRNS chunk data into a full alpha channel
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
	}
	// reduce images with 16-bit samples to 8 bits
	if (bit_depth == 16) {
		png_set_strip_16(png_ptr);
	}
	// expand grayscale images to RGB
	if (color_type == PNG_COLOR_TYPE_GRAY
			|| color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
		png_set_gray_to_rgb(png_ptr);
	}

	bit_depth = 8;
	png_uint_32 rowbytes;
	png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);

	png_read_update_info(png_ptr, info_ptr);

	rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	unsigned char* image_data = new unsigned char[rowbytes * height];
	for (unsigned short i = 0; i < height; ++i) {
//		row_pointers[height - i - 1] = image_data + i * rowbytes;
		row_pointers[i] = image_data + i * rowbytes;
	}
	png_read_image(png_ptr, row_pointers);
	png_read_end(png_ptr, info_ptr);

	png_uint_32 channel = rowbytes / width;
	if (channel == 4) {
		unsigned int *tmp = (unsigned int *) image_data;
		for (unsigned short i = 0; i < height; i++) {
			for (unsigned int j = 0; j < rowbytes; j += 4) {
				*tmp++ = CC_RGB_PREMULTIPLY_ALPHA(row_pointers[i][j],
						row_pointers[i][j + 1], row_pointers[i][j + 2],
						row_pointers[i][j + 3]);
			}
		}

	}

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	free(row_pointers);
	fclose(fp);
	GLPNG* png = new GLPNG();
	png->data = image_data;
	png->width = width;
	png->height = height;
	png->channel = channel;
	return png;
}
GLPNG* PngUtil::readAssets(const char* file) {
	AssetsFile* _file = FileUnit::getAssets(file);
	GLPNG* png=read(_file->data,_file->nSize);
	delete _file;
	return png;

}
GLPNG* PngUtil::read(unsigned char* data, int nDatalen) {
	png_byte header[8] = { 0 };

	// check the data is png or not
	memcpy(header, data, 8);
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) {

		return (NULL);
	}
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
			(png_voidp) NULL, NULL, NULL);
	if (!png_ptr) {

		return (NULL);
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);

	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);

		return (NULL);
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		return (NULL);
	}
	// set the read call back function
	tImageSource imageSource;
	imageSource.data = (unsigned char*) data;
	imageSource.size = nDatalen;
	imageSource.offset = 0;
	png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

	//let libpng know you already read the f bytes
	//png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image datairst 8
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 width, height;

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	//CCLOG("color type %u", color_type);

	// force palette images to be expanded to 24-bit RGB
	// it may include alpha channel
	if (color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
	}
	// low-bit-depth grayscale images are to be expanded to 8 bits
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	}
	// expand any tRNS chunk data into a full alpha channel
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
	}
	// reduce images with 16-bit samples to 8 bits
	if (bit_depth == 16) {
		png_set_strip_16(png_ptr);
	}
	// expand grayscale images to RGB
	if (color_type == PNG_COLOR_TYPE_GRAY
			|| color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
		png_set_gray_to_rgb(png_ptr);
	}

	bit_depth = 8;
	png_uint_32 rowbytes;
	png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);

	png_read_update_info(png_ptr, info_ptr);

	rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	unsigned char* image_data = new unsigned char[rowbytes * height];
	for (unsigned short i = 0; i < height; ++i) {
//		row_pointers[height - i - 1] = image_data + i * rowbytes;
		row_pointers[i] = image_data + i * rowbytes;
	}
	png_read_image(png_ptr, row_pointers);
	png_read_end(png_ptr, info_ptr);

	png_uint_32 channel = rowbytes / width;
	if (channel == 4) {
		unsigned int *tmp = (unsigned int *) image_data;
		for (unsigned short i = 0; i < height; i++) {
			for (unsigned int j = 0; j < rowbytes; j += 4) {
				*tmp++ = CC_RGB_PREMULTIPLY_ALPHA(row_pointers[i][j],
						row_pointers[i][j + 1], row_pointers[i][j + 2],
						row_pointers[i][j + 3]);
			}
		}

	}

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	free(row_pointers);
	GLPNG* png = new GLPNG();
	png->data = image_data;
	png->width = width;
	png->height = height;
	png->channel = channel;
	return png;
}
