package com.gastornisapp.barberpole.ui

import android.opengl.GLES30
import android.util.Log

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