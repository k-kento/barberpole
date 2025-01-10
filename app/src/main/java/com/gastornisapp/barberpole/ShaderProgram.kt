package com.gastornisapp.barberpole

import android.opengl.GLES30
import android.util.Log

class ShaderProgram {

    var program: Int? = null

    fun createProgram() {
        // シェーダーのコンパイルとプログラムの作成
        val vertexShader = loadShader(GLES30.GL_VERTEX_SHADER, VERTEX_SHADER_CODE)
        val fragmentShader = loadShader(GLES30.GL_FRAGMENT_SHADER, FRAGMENT_SHADER_CODE)

        program = GLES30.glCreateProgram().also {
            GLES30.glAttachShader(it, vertexShader)
            GLES30.glAttachShader(it, fragmentShader)
            GLES30.glLinkProgram(it)
        }
    }

    fun useProgram() {
        program?.let {
            GLES30.glUseProgram(it)
        }
    }

    // シェーダーをロードしてコンパイル
    private fun loadShader(type: Int, shaderCode: String): Int {
        val shader = GLES30.glCreateShader(type)
        GLES30.glShaderSource(shader, shaderCode)
        GLES30.glCompileShader(shader)
        // コンパイルエラーのチェック
        val compileStatus = IntArray(1)
        GLES30.glGetShaderiv(shader, GLES30.GL_COMPILE_STATUS, compileStatus, 0)
        if (compileStatus[0] == 0) {
            Log.e(TAG, "[HERE ->] " + GLES30.glGetShaderInfoLog(shader))
            Log.e(TAG, GLES30.glGetShaderSource(shader))
            throw RuntimeException("Shader compilation failed")
        }
        return shader
    }

    companion object {
        private const val VERTEX_SHADER_CODE = """
        uniform mat4 uModelViewMatrix;
        attribute vec4 vPosition;
        attribute vec4 aColor;
        varying vec4 vColor;
        
        void main() {
            gl_Position = uModelViewMatrix * vPosition;
            vColor = aColor;
        }
    """

        private const val FRAGMENT_SHADER_CODE = """
        precision mediump float;
        varying vec4 vColor;

        void main() {
            gl_FragColor = vColor;
        }
    """

        private const val TAG = "ShaderProgram"
    }
}