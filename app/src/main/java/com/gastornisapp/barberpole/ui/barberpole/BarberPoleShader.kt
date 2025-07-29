package com.gastornisapp.barberpole.ui.barberpole

import android.opengl.GLES30
import com.gastornisapp.barberpole.ui.gl.shader.ShaderProgram

class BarberPoleShader :
    ShaderProgram(vertexShaderFile = "shaders/color_transform.vsh", fragmentShaderFile = "shaders/pass_through.fsh") {

    override fun onInitialized(program: Int) {
        modelLocation = GLES30.glGetUniformLocation(program, "u_Model")
        positionLocation = GLES30.glGetAttribLocation(program, "a_Position")
        colorLocation = GLES30.glGetAttribLocation(program, "a_Color")
    }

    var modelLocation = -1
    var positionLocation = -1
    var colorLocation = -1
}