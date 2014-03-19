package com.alibaba.android.babylon.graphic;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteOrder;

import android.graphics.Bitmap;
import android.opengl.ETC1Util;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.util.Log;

public class LWGLHelper {
	
	public static String TAG = "lwglHelper";
    public static void checkGlError(String string) {
        int error = GLES20.glGetError();
        if (error != GLES20.GL_NO_ERROR) {
            Log.e(TAG, string + ": glError " + error);
            throw new RuntimeException(string + ": glError " + error);
        }
    }

    public static int makeBitmapTexture(Bitmap bitmap) {
        if (bitmap == null) {
            return 0;
        }
//        checkGlError("makeBitmapTexture");
        int[] value = new int[1];
        GLES20.glGenTextures(1, value, 0);
        int texture = value[0];
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texture);
        try {
            GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);
            GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
            GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
//            checkGlError("makeBitmapTexture");
        } catch (IllegalArgumentException exception) {
            Log.d(TAG, "Failed to load GL texture", exception);
            return 0;
        }
        return texture;
    }

    public static int makeETC1Texture(InputStream is) {
        int[] textures = new int[1];
        GLES20.glGenTextures(1, textures, 0);
        int texture = textures[0];
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texture);
        try {
            ETC1Util.ETC1Texture ect1Texture = ETC1Util.createTexture(is);
            ETC1Util.loadTexture(GLES20.GL_TEXTURE_2D, 0, 0, GLES20.GL_RGB, GLES20.GL_UNSIGNED_SHORT_5_6_5, ect1Texture);
            GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
            GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
            return texture;
        } catch (IOException exception) {
            throw new RuntimeException("makeETC1Texture " + exception.getStackTrace());
        }
    }

    public static int[] convertRGBA_8888toARGB_8888(final int[] pPixelsRGBA_8888) {
        return LWGLHelper.convertRGBA_8888toARGB_8888(pPixelsRGBA_8888, ByteOrder.nativeOrder());
    }

    public static int[] convertRGBA_8888toARGB_8888(final int[] pPixelsRGBA_8888, final ByteOrder pByteOrder) {
        if (pByteOrder == ByteOrder.LITTLE_ENDIAN) {
            for (int i = pPixelsRGBA_8888.length - 1; i >= 0; i--) {
                final int pixel = pPixelsRGBA_8888[i];
                /* [A][B][G][R] to [A][R][G][B] */
                /* From : [ A7 A6 A5 A4 A3 A2 A1 A0 | B7 B6 B5 B4 B3 B2 B1 B0 | G7 G6 G5 G4 G3 G2 G1 G0 | R7 R6 R5 R4 R3 R2 R1 R0 ] */
                /* To : [ A7 A6 A5 A4 A3 A2 A1 A0 | R7 R6 R5 R4 R3 R2 R1 R0 | G7 G6 G5 G4 G3 G2 G1 G0 | B7 B6 B5 B4 B3 B2 B1 B0 ] */
                pPixelsRGBA_8888[i] = (pixel & 0xFF00FF00) | ((pixel << 16) & 0x00FF0000) | ((pixel >> 16) & 0x000000FF);
            }
        } else {
            for (int i = pPixelsRGBA_8888.length - 1; i >= 0; i--) {
                final int pixel = pPixelsRGBA_8888[i];
                /* [R][G][B][A] to [A][R][G][B] */
                /* From : [ R7 R6 R5 R4 R3 R2 R1 R0 | G7 G6 G5 G4 G3 G2 G1 G0 | B7 B6 B5 B4 B3 B2 B1 B0 | A7 A6 A5 A4 A3 A2 A1 A0 ] */
                /* To : [ A7 A6 A5 A4 A3 A2 A1 A0 | R7 R6 R5 R4 R3 R2 R1 R0 | G7 G6 G5 G4 G3 G2 G1 G0 | B7 B6 B5 B4 B3 B2 B1 B0 ] */
                pPixelsRGBA_8888[i] = ((pixel >> 8) & 0x00FFFFFF) | ((pixel << 24) & 0xFF000000);
            }
        }
        return pPixelsRGBA_8888;
    }
}