package com.gastornisapp.barberpole.ui.gl.mesh

import android.opengl.GLES30
import java.nio.ByteBuffer
import java.nio.ByteOrder

class SquareMesh(
    positionLocation: Int,
    texCordLocation: Int
) {
    companion object {
        private val VERTICES = floatArrayOf(
            -1f, -1f, 0f, 1f, // 左下
            1f, -1f, 1f, 1f, // 右下
            -1f, 1f, 0f, 0f, // 左上
            1f, 1f, 1f, 0f  // 右上
        )

        private val INDICES = shortArrayOf(
            0, 1, 2,
            2, 1, 3
        )
    }

    private val vao = IntArray(1)
    private val vbo = IntArray(1)
    private val ibo = IntArray(1)

    init {
        GLES30.glGenVertexArrays(1, vao, 0)
        GLES30.glGenBuffers(1, vbo, 0)
        GLES30.glGenBuffers(1, ibo, 0)

        val vaoId = vao[0]
        GLES30.glBindVertexArray(vaoId)

        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, vbo[0])
        val vertexBuffer = ByteBuffer.allocateDirect(VERTICES.size * 4)
            .order(ByteOrder.nativeOrder()).asFloatBuffer().apply {
                put(VERTICES)
                position(0)
            }
        GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, VERTICES.size * 4, vertexBuffer, GLES30.GL_STATIC_DRAW)

        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, ibo[0])
        val indexBuffer = ByteBuffer.allocateDirect(INDICES.size * 2)
            .order(ByteOrder.nativeOrder()).asShortBuffer().apply {
                put(INDICES)
                position(0)
            }
        GLES30.glBufferData(GLES30.GL_ELEMENT_ARRAY_BUFFER, INDICES.size * 2, indexBuffer, GLES30.GL_STATIC_DRAW)

        GLES30.glEnableVertexAttribArray(positionLocation)
        GLES30.glVertexAttribPointer(positionLocation, 2, GLES30.GL_FLOAT, false, 4 * 4, 0)

        GLES30.glEnableVertexAttribArray(texCordLocation)
        GLES30.glVertexAttribPointer(texCordLocation, 2, GLES30.GL_FLOAT, false, 4 * 4, 2 * 4)

        GLES30.glBindVertexArray(0)
    }

    fun draw() {
        GLES30.glBindVertexArray(vao[0])
        GLES30.glDrawElements(GLES30.GL_TRIANGLES, INDICES.size, GLES30.GL_UNSIGNED_SHORT, 0)
        GLES30.glBindVertexArray(0)
    }

    fun release() {
        GLES30.glDeleteVertexArrays(1, vao, 0)
        GLES30.glDeleteBuffers(1, vbo, 0)
        GLES30.glDeleteBuffers(1, ibo, 0)
    }
}