package com.gastornisapp.barberpole.ui

import android.content.Context
import android.graphics.BitmapFactory
import android.opengl.GLES30
import android.opengl.GLUtils
import android.util.Log
import androidx.annotation.DrawableRes

private const val TAG = "GlUtil"

// シェーダーをロードしてコンパイル
fun loadShader(type: Int, shaderCode: String): Int {
    val shader = GLES30.glCreateShader(type)
    if (shader == 0) {
        throw RuntimeException("Failed to create shader of type: $type")
    }

    // 2. シェーダーのソースコードを設定してコンパイル
    GLES30.glShaderSource(shader, shaderCode)
    GLES30.glCompileShader(shader)


    // 3. コンパイル結果を取得
    val compileStatus = IntArray(1)
    GLES30.glGetShaderiv(shader, GLES30.GL_COMPILE_STATUS, compileStatus, 0)

    if (compileStatus[0] == 0) {
        val shaderTypeStr = when (type) {
            GLES30.GL_VERTEX_SHADER -> "VERTEX"
            GLES30.GL_FRAGMENT_SHADER -> "FRAGMENT"
            else -> "UNKNOWN"
        }
        Log.e(TAG, "[$shaderTypeStr SHADER ERROR]")
        Log.e(TAG, GLES30.glGetShaderInfoLog(shader))

        // メモリリーク防止のためシェーダーを削除
        GLES30.glDeleteShader(shader)
        throw RuntimeException("$shaderTypeStr shader compilation failed")
    }

    return shader
}

fun loadTexture(context: Context, @DrawableRes resId: Int): Int {
    // 1. テクスチャIDを生成
    val textureIds = IntArray(1)
    GLES30.glGenTextures(1, textureIds, 0)

    if (textureIds[0] == 0) {
        throw RuntimeException("Failed to generate texture ID")
    }

    // 2. Bitmapを読み込む
    val bitmap = BitmapFactory.decodeResource(context.resources, resId)

    // 3. テクスチャをバインド
    GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureIds[0])

    // 4. テクスチャのパラメータ設定
    // 線形補間で画像を拡大縮小する
    GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR)
    GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR)
    // 境界外の UV に対して端の色を使う
    GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE)
    GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE)

    // 5. BitmapをOpenGLのテクスチャにアップロード
    GLUtils.texImage2D(GLES30.GL_TEXTURE_2D, 0, bitmap, 0)

    // 6. Bitmapを解放
    bitmap?.recycle()

    // 7. バインド解除
    GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0)

    return textureIds[0]
}