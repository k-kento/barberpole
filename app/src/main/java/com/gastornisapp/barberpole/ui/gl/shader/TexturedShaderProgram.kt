package com.gastornisapp.barberpole.ui.gl.shader

import android.opengl.GLES30

class TexturedShaderProgram : ShaderProgram(vertexShaderFile = "shaders/texture.vsh", fragmentShaderFile = "shaders/texture.fsh") {

    var uMvpMatrixLocation = -1
    var aPosition = -1
    var uTextureLocation: Int = -1
    var aTexCoord = -1
    var uColorLocation = -1

    override fun onInitialized(program: Int) {
        aPosition = GLES30.glGetAttribLocation(program, "a_Position")
        uTextureLocation = GLES30.glGetUniformLocation(program, "u_Texture")
        uMvpMatrixLocation = GLES30.glGetUniformLocation(program, "u_MvpMatrix")
        aTexCoord = GLES30.glGetAttribLocation(program, "a_TexCoord")
        uColorLocation = GLES30.glGetUniformLocation(program, "u_Color")
    }
}