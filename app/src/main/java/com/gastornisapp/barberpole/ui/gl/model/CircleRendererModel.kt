package com.gastornisapp.barberpole.ui.gl.model

import android.opengl.GLES30
import com.gastornisapp.barberpole.ui.gl.shader.CircleShaderProgram
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer

class CircleRendererModel {

    private val vao = IntArray(1)
    private val vbo = IntArray(1)

    private lateinit var program: CircleShaderProgram

    // 四角形の頂点データ (x, y, u, v)
    private val vertexData = floatArrayOf(
        -1f, -1f, 0f, 0f,
        1f, -1f, 1f, 0f,
        -1f, 1f, 0f, 1f,
        1f, 1f, 1f, 1f
    )

    private val vertexBuffer: FloatBuffer = ByteBuffer.allocateDirect(vertexData.size * 4)
        .order(ByteOrder.nativeOrder())
        .asFloatBuffer()
        .apply {
            put(vertexData)
            position(0)
        }

    fun initialize(program: CircleShaderProgram) {
        this.program = program

        GLES30.glGenVertexArrays(1, vao, 0)
        GLES30.glGenBuffers(1, vbo, 0)

        GLES30.glBindVertexArray(vao[0])
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, vbo[0])

        GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, vertexData.size * 4, vertexBuffer, GLES30.GL_STATIC_DRAW)

        program.setupAttributes()

        GLES30.glBindVertexArray(0)
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, 0)
    }

    fun draw(mvpMatrix: FloatArray, color: FloatArray) {
        program.useProgram()
        program.setColor(color)
        program.setMvpMatrix(mvpMatrix)

        GLES30.glBindVertexArray(vao[0])
        GLES30.glDrawArrays(GLES30.GL_TRIANGLE_STRIP, 0, 4)
        GLES30.glBindVertexArray(0)
    }

    fun release() {
        GLES30.glDeleteBuffers(1, vbo, 0)
        GLES30.glDeleteVertexArrays(1, vao, 0)
    }

    companion object {
        const val SIZE = 2
    }
}