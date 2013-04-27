package com.example.gltest;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Menu;
import android.view.Window;
import android.view.WindowManager;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		GLSurfaceView v = new GLSurfaceView(this);
		v.setRenderer(new GLView());
		setContentView(v);
		final ApplicationInfo applicationInfo = this.getApplicationInfo();
		GLInterface.setApkSource(applicationInfo.sourceDir);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	class GLView implements GLSurfaceView.Renderer {
		int id;
//		int bid;
//		int t=0;
//		private EGL10 egl;

		@Override
		public void onDrawFrame(GL10 gl) {
			GLInterface.render();
//			GLInterface.drawImage(id,0,0,0,0,0,0,0,800,480);
			GLInterface.playAnimation((short)10409);
//			GLInterface.drawBuffer(bid);
////			GLInterface.drawImage(id,0,0);
//			GLInterface.openAlpha(255);
//			GLInterface.drawImage(id,150, 150);
//			GLInterface.closeAlpha();
//			GLInterface.endDrawBuffer();
//			GLInterface.renderBuffer(bid);
		}

		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			GLInterface.init(width, height);
//			bid=GLInterface.createBuffer(512, 512);
			id=GLInterface.createImageAssets("assets/image/1.png");
			GLInterface.createAnimationAssets("assets/ani/10409.ani");
		}

		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {

		}

	}

}
