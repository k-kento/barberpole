package com.gastornisapp.barberpole.ui.gl.shader

import android.opengl.GLES30

class CircleShaderProgram : ShaderProgram(vertexShaderFile = "shaders/circle.vsh", fragmentShaderFile = "shaders/circle.fsh") {

    private var uMvpMatrixLocation = -1
    private var uColorLocation = -1

    override fun onInitialized(program: Int) {
        uMvpMatrixLocation = GLES30.glGetUniformLocation(program, "u_MvpMatrix")
        uColorLocation = GLES30.glGetUniformLocation(program, "u_Color")
    }

    fun setupAttributes() {
        val aPositionLocation = GLES30.glGetAttribLocation(program, "a_Position")
        GLES30.glEnableVertexAttribArray(aPositionLocation)
        GLES30.glVertexAttribPointer(aPositionLocation, 2, GLES30.GL_FLOAT, false, 4 * 4, 0)

        val aTexCoordLocation = GLES30.glGetAttribLocation(program, "a_TexCoord")
        GLES30.glEnableVertexAttribArray(aTexCoordLocation)
        GLES30.glVertexAttribPointer(aTexCoordLocation, 2, GLES30.GL_FLOAT, false, 4 * 4, 2 * 4)
    }

    fun setMvpMatrix(mvpMatrix: FloatArray) {
        GLES30.glUniformMatrix4fv(uMvpMatrixLocation, 1, false, mvpMatrix, 0)
    }

    fun setColor(color: FloatArray) {
        GLES30.glUniform3fv(uColorLocation, 1, color, 0)
    }
}