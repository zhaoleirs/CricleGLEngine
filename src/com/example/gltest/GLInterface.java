package com.example.gltest;

public class GLInterface {
	native static public void init(int width, int height);
	native static public void render();
	native static public int createImageAssets(String path);
	native static public int createImage(String path);
	native static public void createAnimationAssets(String path);
	native static public void createAnimation(String path);
	native static public void drawImage(int id,int flip,int sx,int sy,int sw,int sh,int dx,int dy,int dw,int dh);
	native static public void playAnimation(short id);
	native static public void openAlpha(int alpha);
	native static public void closeAlpha();
	native static public int createBuffer(int w,int h);
	native static public void drawBuffer(int id);
	native static public void endDrawBuffer();
	native static public void renderBuffer(int id);
	native static public void setApkSource(String apk);
	static public void drawImage(int id,int x,int y){
		drawImage(id, 0, 0, 0, 0, 0, x, y, 0, 0);
	}
	static{
		System.loadLibrary("sewisdomGL");
	}
	
}
