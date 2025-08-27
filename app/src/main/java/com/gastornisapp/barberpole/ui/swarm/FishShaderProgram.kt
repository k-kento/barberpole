package com.gastornisapp.barberpole.ui.swarm

import android.opengl.GLES30
import com.gastornisapp.barberpole.ui.gl.shader.ShaderProgram

class FishShaderProgram : ShaderProgram(
    vertexShaderFile = "shaders/fish.vsh",
    fragmentShaderFile = "shaders/fish.fsh"
) {

    var posHandle = -1
    private var mvpMatrixHandle = -1

    override fun onInitialized(program: Int) {
        posHandle = GLES30.glGetAttribLocation(program, "aPosition")
        mvpMatrixHandle = GLES30.glGetUniformLocation(program, "uMvpMatrix")
    }

    fun setMvpMatrix(mvpMatrix: FloatArray) {
        GLES30.glUniformMatrix4fv(mvpMatrixHandle, 1, false, mvpMatrix, 0)
    }
}