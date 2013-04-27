#include <GLES/gl.h>
#include <GLES/glext.h>
#include "com_example_gltest_GLInterface.h"
#include <android/log.h>
#include "PngUtil.h"
#include "Texture.h"
#include "GLBuffer.h"
#include "Common.h"
#include "Ani.h"

extern "C" {
/*
 * Class:     com_example_gltest_GLInterface
 * Method:    init
 * Signature: (II)V
 */

static int computeW = 0;
static int computeH = 0;

JNIEXPORT void JNICALL Java_com_example_gltest_GLInterface_init(JNIEnv *env,
		jclass obj, jint w, jint h) {
	screenW = w;
	screenH = h;
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glDisable(GL_DITHER);
	glDisable(GL_LIGHTING);
	glViewport(0, 0, w, h);
	glOrthof(0, w, 0, h, 0, 5);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

}

/*
 * Class:     com_example_gltest_GLInterface
 * Method:    render
 * Signature: ()V
 */JNIEXPORT void JNICALL Java_com_example_gltest_GLInterface_render(JNIEnv *env,
		jclass obj) {
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, NULL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	computeW = screenW;
	computeH = screenH;
	//	glBindTexture(GL_TEXTURE_2D, texture->texture);
	//	int cropRect[4] = { 0, 0, texture->width, texture->height };
	//	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, cropRect);
	//	glDisable(GL_BLEND);
	//	glDrawTexiOES(0, 0, 0, 700, 1200);
	//	glEnable(GL_BLEND);
	//	glDrawTexiOES(0, 0, 0, 512, 512);

}
/*
 * Class:     com_example_gltest_GLInterface
 * Method:    drawImage
 * Signature: (IIIIIIIIII)V
 */JNIEXPORT void JNICALL Java_com_example_gltest_GLInterface_drawImage(
		JNIEnv *env, jclass obj, jint id, jint flip, jint sx, jint sy, jint sw,
		jint sh, jint dx, jint dy, jint dw, jint dh) {
	Texture::shareTexture(id)->draw(sx, sy, sw, sh, dx, computeH - dh - dy, dw,
			dh);

}

/*
 * Class:     com_example_gltest_GLInterface
 * Method:    createImageAssets
 * Signature: (Ljava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_com_example_gltest_GLInterface_createImageAssets(
		JNIEnv *env, jclass obj, jstring path) {
	Texture* texture = Texture::create();
	const char* _path = env->GetStringUTFChars(path, JNI_FALSE);
	GLPNG* png = PngUtil::readAssets(_path);
	texture->bind(png->data, png->width, png->height, png->channel);
	texture->load();
	delete png;
	delete[] _path;
	return texture->texture;
}
/*
 * Class:     com_example_gltest_GLInterface
 * Method:    createImage
 * Signature: (Ljava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_com_example_gltest_GLInterface_createImage(
		JNIEnv *env, jclass obj, jstring path) {
	Texture* texture = Texture::create();
	const char* _path = env->GetStringUTFChars(path, JNI_FALSE);
	GLPNG* png = PngUtil::read(_path);
	texture->bind(png->data, png->width, png->height, png->channel);
	texture->load();
	delete png;
	delete[] _path;
	return texture->texture;
}
/*
 * Class:     com_example_gltest_GLInterface
 * Method:    openAlpha
 * Signature: (I)V
 */JNIEXPORT void JNICALL Java_com_example_gltest_GLInterface_openAlpha(
		JNIEnv * env, jclass obj, jint alpha) {
	glEnable(GL_BLEND);
	glColor4f(1, 1, 1, alpha / 255.0);
}

/*
 * Class:     com_example_gltest_GLInterface
 * Method:    closeAlpha
 * Signature: ()V
 */JNIEXPORT void JNICALL Java_com_example_gltest_GLInterface_closeAlpha(JNIEnv *,
		jclass) {
	glDisable(GL_BLEND);
}
/*
 * Class:     com_example_gltest_GLInterface
 * Method:    createBuffer
 * Signature: (II)I
 */JNIEXPORT jint JNICALL Java_com_example_gltest_GLInterface_createBuffer(
		JNIEnv *env, jclass obj, jint w, jint h) {
	GLBuffer* buffer = GLBuffer::create(w,h);
	return buffer->frame;

}

/*
 * Class:     com_example_gltest_GLInterface
 * Method:    drawBuffer
 * Signature: (I)V
 */JNIEXPORT void JNICALL Java_com_example_gltest_GLInterface_drawBuffer(JNIEnv *,
		jclass, jint id) {
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, id);
	glClear(GL_DEPTH_BUFFER_BIT);
	GLBuffer* buff = GLBuffer::shareBuffer(id);
	computeH = buff->height;
}

/*
 * Class:     com_example_gltest_GLInterface
 * Method:    endDrawBuffer
 * Signature: ()V
 */JNIEXPORT void JNICALL Java_com_example_gltest_GLInterface_endDrawBuffer(
		JNIEnv *, jclass) {
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, NULL);
	computeH = screenH;
}

/*
 * Class:     com_example_gltest_GLInterface
 * Method:    renderBuffer
 * Signature: (I)V
 */JNIEXPORT void JNICALL Java_com_example_gltest_GLInterface_renderBuffer(
		JNIEnv *, jclass, jint id) {
	GLBuffer* buff = GLBuffer::shareBuffer(id);
	glBindTexture(GL_TEXTURE_2D, buff->texture->texture);
	int cropRect[4] = { 0, 0, buff->width, buff->height };
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, cropRect);
	glDrawTexiOES(0, screenH - buff->height, 0, buff->width, buff->height);
}

/*
 * Class:     com_example_gltest_GLInterface
 * Method:    setApkSource
 * Signature: (Ljava/lang/String;)V
 */JNIEXPORT void JNICALL Java_com_example_gltest_GLInterface_setApkSource(
		JNIEnv *env, jclass obj, jstring path) {
	const char* _path = env->GetStringUTFChars(path, JNI_FALSE);
	Common::APK_PATH = _path;
}
/*
 * Class:     com_example_gltest_GLInterface
 * Method:    createAnimationAssets
 * Signature: (Ljava/lang/String;)S
 */JNIEXPORT void JNICALL Java_com_example_gltest_GLInterface_createAnimationAssets(
		JNIEnv *env, jclass obj, jstring path) {
	const char* _path = env->GetStringUTFChars(path, JNI_FALSE);
	Ani::loadAeests(_path);
	delete _path;
}
/*
 * Class:     com_example_gltest_GLInterface
 * Method:    createAnimation
 * Signature: (Ljava/lang/String;)S
 */JNIEXPORT void JNICALL Java_com_example_gltest_GLInterface_createAnimation(
		JNIEnv *, jclass, jstring){

 }
/*
 * Class:     com_example_gltest_GLInterface
 * Method:    playAnimation
 * Signature: (S)I
 */JNIEXPORT void JNICALL Java_com_example_gltest_GLInterface_playAnimation(
		JNIEnv *, jclass, jshort id) {
	 if(Ani::shareAni(id)){
		 Ani::shareAni(id)->play(2,true);
	 }
}
}

