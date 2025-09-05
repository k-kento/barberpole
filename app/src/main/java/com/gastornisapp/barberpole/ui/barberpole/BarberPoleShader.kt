package com.gastornisapp.barberpole.ui.barberpole

import android.opengl.GLES30
import com.gastornisapp.barberpole.ui.gl.shader.ShaderProgram

class BarberPoleShader : ShaderProgram(
    vertexShaderFile = "shaders/color_transform.vsh",
    fragmentShaderFile = "shaders/pass_through.fsh"
) {

    private var mvpModelLocation = -1
    var positionLocation = -1
    var colorLocation = -1

    override fun onInitialized(program: Int) {
        mvpModelLocation = GLES30.glGetUniformLocation(program, "u_MvpModel")
        colorLocation = GLES30.glGetAttribLocation(program, "a_Color")
        positionLocation = GLES30.glGetAttribLocation(program, "a_Position")
    }

    fun setMvpModel(matrix: FloatArray) {
        GLES30.glUniformMatrix4fv(mvpModelLocation, 1, false, matrix, 0)
    }
}
