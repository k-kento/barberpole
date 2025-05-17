package com.gastornisapp.barberpole.ui.barberpole

import android.opengl.GLES30
import com.gastornisapp.barberpole.ui.loadShader

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
    }
}