package com.gastornisapp.barberpole.ui.gl.shader

import android.opengl.GLES30

class TexturedTintShaderProgram : TexturedShaderProgram(fragmentShaderFile = "shaders/texture_tint.fsh") {

    private var uColorLocation = -1

    override fun onInitialized(program: Int) {
        super.onInitialized(program)
        uColorLocation = GLES30.glGetUniformLocation(program, "u_Color")
    }

    fun setColor(color: FloatArray) {
        GLES30.glUniform4fv(uColorLocation, 1, color, 0)
    }
}
