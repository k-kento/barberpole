package com.gastornisapp.barberpole.ui.gl.model

import android.content.Context
import android.opengl.GLES30
import com.gastornisapp.barberpole.ui.loadFragmentShader
import com.gastornisapp.barberpole.ui.loadVertexShader

abstract class ShaderProgram(val vertexShaderFile: String, val fragmentShaderFile: String) {

    var program: Int = -1

    fun initialize(context: Context) {
        initialize(context, vertexShaderFile, fragmentShaderFile)
        onInitialized(program)
    }

    abstract fun onInitialized(program: Int)

    private fun initialize(context: Context, vertexShaderFile: String, fragmentShaderFile: String) {
        val asset = context.assets
        val vertexShaderSrc = asset.open(vertexShaderFile).bufferedReader(Charsets.UTF_8).use { it.readText() }
        val vertexShader = loadVertexShader(vertexShaderSrc)

        val fragmentShaderSrc = asset.open(fragmentShaderFile).bufferedReader(Charsets.UTF_8).use { it.readText() }
        val fragmentShader = loadFragmentShader(fragmentShaderSrc)

        program = GLES30.glCreateProgram().also {
            GLES30.glAttachShader(it, vertexShader)
            GLES30.glAttachShader(it, fragmentShader)
            GLES30.glLinkProgram(it)
        }
    }

    fun useProgram() {
        GLES30.glUseProgram(program)
    }
}