package com.example.android_opengles;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import android.app.Activity;
import android.os.Bundle;
import android.util.Xml;

import com.alibaba.android.babylon.graphic.LWGLAssetsUtil2;
import com.alibaba.android.babylon.graphic.LWGLView;

public class MainActivity extends Activity {

	private LWGLView mEGLView;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mEGLView = (LWGLView) findViewById(R.id.eglView);
		
		String lomoFilter = LWGLAssetsUtil2.getAssetsShader(this, "lomo");
	}
//	
//	public List parse(InputStream in) throws XmlPullParserException, IOException {
//		try {
//			XmlPullParser parser = Xml.newPullParser();
//			parser.setFeature(XmlPullParser.FEATURE_PROCESS_NAMESPACES, false);
//			parser.setInput(in, null);
//			parser.nextTag();
//			return readFeed(parser);
//		} finally {
//			in.close();
//		}
//	}


}
