package com.gastornisapp.barberpole.ui.gl.shader

import android.opengl.GLES30

open class TexturedShaderProgram(fragmentShaderFile: String = "shaders/texture.fsh") :
    ShaderProgram(vertexShaderFile = "shaders/texture.vsh", fragmentShaderFile) {

    var aPosition = -1
        private set
    var aTexCoord = -1
        private set
    private var uTextureLocation: Int = -1
    private var uMvpMatrixLocation = -1

    override fun onInitialized(program: Int) {
        aPosition = GLES30.glGetAttribLocation(program, "a_Position")
        uTextureLocation = GLES30.glGetUniformLocation(program, "u_Texture")
        uMvpMatrixLocation = GLES30.glGetUniformLocation(program, "u_MvpMatrix")
        aTexCoord = GLES30.glGetAttribLocation(program, "a_TexCoord")
    }

    fun setMvpMatrix(mvpMatrix: FloatArray) {
        GLES30.glUniformMatrix4fv(uMvpMatrixLocation, 1, false, mvpMatrix, 0)
    }

    fun setTextureUnit(textureUnit: Int) {
        GLES30.glUniform1i(uTextureLocation, textureUnit)
    }
}
