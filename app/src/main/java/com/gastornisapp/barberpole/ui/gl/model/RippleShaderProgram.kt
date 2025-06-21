package com.gastornisapp.barberpole.ui.gl.model

import android.opengl.GLES30

class RippleShaderProgram : ShaderProgram(vertexShaderFile = "shaders/ripple.vsh", fragmentShaderFile = "shaders/ripple.fsh") {

    private var uMvpMatrixLocation: Int = -1
    private var uTImeLocation: Int = -1
    private var uColorLocation: Int = -1

    override fun onInitialized(program: Int) {
        uMvpMatrixLocation = GLES30.glGetUniformLocation(program, "u_MvpMatrix")
        uTImeLocation = GLES30.glGetUniformLocation(program, "u_Time")
        uColorLocation = GLES30.glGetUniformLocation(program, "u_Color")
    }

    fun setMvpMatrix(mvpMatrix: FloatArray) {
        GLES30.glUniformMatrix4fv(uMvpMatrixLocation, 1, false, mvpMatrix, 0)
    }

    fun setTime(time: Float) {
        GLES30.glUniform1f(uTImeLocation, time)
    }

    fun setColor(color: FloatArray) {
        GLES30.glUniform3fv(uColorLocation, 1, color, 0)
    }
}