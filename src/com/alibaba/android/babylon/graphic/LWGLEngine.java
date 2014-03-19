package com.alibaba.android.babylon.graphic;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.util.Log;

public class LWGLEngine {

	public static String TAG = "lwglEngine";
	static {
		System.loadLibrary("lwglEngine");
	}


    public enum RotationMode {
        UnknowRotation(-1),
        Rotate0(0x00),
        Rotate90(0x10),
        Rotate180(0x20),
        Rotate270(0x30),
        Rotate0FlipV(0x01), 	// Flip vertical
        Rotate90FlipV(0x11), 	// Rotate 90 then flip vertical, AKA transpose, flipped about top-left <--> bottom-right axis
        Rotate180FlipV(0x21), 	// Rotate 180 then flip vertical, AKA flip horizontal
        Rotate270FlipV(0x31); 	// Rotate 270 then flip vertical, AKA transverse, flipped about top-right <--> bottom-left axis

        private final int mValue;

        private RotationMode(int v) {
            mValue = v;
        }

        public int getRotation() {
            return ((mValue & 0xF0) >> 4) * 90;
        }

        public int getFlipVertical() {
            return (mValue & 0x0F);
        }

        public boolean isFlipVertical() {
            return (mValue & 0x0F) == 1;
        }

        public int getValue() {
            return mValue;
        }

        public static RotationMode merge(RotationMode r1, RotationMode r2) {
            int rotation = r1.getRotation() + (r1.getFlipVertical() == 1 ? -r2.getRotation() : r2.getRotation());
            int value = (((rotation % 360) / 90) << 4) | (r1.getFlipVertical() ^ r2.getFlipVertical());
            return fromValue(value);
        }

        public static RotationMode fromValue(int value) {
            for (RotationMode mode : RotationMode.values()) {
                if (mode.mValue == value) {
                    return mode;
                }
            }
            return UnknowRotation;
        }
    }
    
	public static Context sContext;
	
	public static native int onSurfaceCreated();

	public static native int onSurfaceChanged(int width, int height);

	public static native int onDrawFrame();
	
	
	public static native int setInputSource(int texture, int width, int height, int rotation);

	public static int createTexture(String path) {
		if (sContext == null) {
			return 0;
		}

		int texture = 0;
		InputStream inputStream = null;
		try {
			if (path.startsWith("/")) {
				inputStream = sContext.getContentResolver().openInputStream(Uri.fromFile(new File(path)));
			} else {
				inputStream = sContext.getAssets().open(path);
			}

			if (inputStream != null) {
				if (!path.endsWith(".pkm")) {
					Bitmap bitmap = BitmapFactory.decodeStream(inputStream);
					LWGLHelper.checkGlError("createTexture " + path);
					texture = LWGLHelper.makeBitmapTexture(bitmap);
					bitmap.recycle();
				} else {
					texture = LWGLHelper.makeETC1Texture(inputStream);
				}
			}
		} catch (IOException exception) {
			Log.e(TAG, "createTexture Failed to open input stream:" + path, exception);
		} finally {
			if (inputStream != null) {
				try {
					inputStream.close();
				} catch (IOException exception) {
					Log.e(TAG, "createTexture Failed to close input stream:" + path, exception);
				}
			}
		}

		return texture;
	}

}
