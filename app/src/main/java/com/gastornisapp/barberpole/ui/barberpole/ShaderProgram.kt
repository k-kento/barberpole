package com.gastornisapp.barberpole.ui.barberpole

import android.opengl.GLES30
import com.gastornisapp.barberpole.ui.loadShader

class ShaderProgram {

    private var program: Int = -1
    var modelLocation = -1
    var positionLocation = -1
    var colorLocation = -1

    fun initialize() {
        // シェーダーのコンパイルとプログラムの作成
        val vertexShader = loadShader(GLES30.GL_VERTEX_SHADER, VERTEX_SHADER_CODE)
        val fragmentShader = loadShader(GLES30.GL_FRAGMENT_SHADER, FRAGMENT_SHADER_CODE)

        program = GLES30.glCreateProgram().also {
            GLES30.glAttachShader(it, vertexShader)
            GLES30.glAttachShader(it, fragmentShader)
            GLES30.glLinkProgram(it)
        }

        GLES30.glUseProgram(program)

        modelLocation  = GLES30.glGetUniformLocation(program , "u_Model")
        positionLocation = GLES30.glGetAttribLocation(program, "a_Position")
        colorLocation = GLES30.glGetAttribLocation(program, "a_Color")
    }

    companion object {
        private const val VERTEX_SHADER_CODE = """
        uniform mat4 u_Model;
        attribute vec4 a_Position;
        attribute vec4 a_Color;
        varying vec4 v_Color;
        
        void main() {
            gl_Position = u_Model * a_Position;
            v_Color = a_Color;
        }
    """

        private const val FRAGMENT_SHADER_CODE = """
        precision mediump float;
        varying vec4 v_Color;

        void main() {
            gl_FragColor = v_Color;
        }
    """
    }
}