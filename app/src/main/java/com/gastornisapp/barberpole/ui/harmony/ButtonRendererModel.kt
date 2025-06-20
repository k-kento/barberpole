package com.gastornisapp.barberpole.ui.harmony

import android.opengl.GLES30
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer

class ButtonRendererModel {

    private val vao = IntArray(1)
    private val vbo = IntArray(1)

    private var program = -1
    private var uMvpMatrixLocation = -1
    private var uColorLocation = -1
    private var uRadiusLocation = -1
    private var uSmoothLocation = -1

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

    fun initialize(program: Int) {
        this.program = program
        uMvpMatrixLocation = GLES30.glGetUniformLocation(program, "u_MvpMatrix")
        uColorLocation = GLES30.glGetUniformLocation(program, "u_Color")
        uRadiusLocation = GLES30.glGetUniformLocation(program, "u_Radius")
        uSmoothLocation = GLES30.glGetUniformLocation(program, "u_Smooth")

        GLES30.glGenVertexArrays(1, vao, 0)
        GLES30.glGenBuffers(1, vbo, 0)

        GLES30.glBindVertexArray(vao[0])
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, vbo[0])

        GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, vertexData.size * 4, vertexBuffer, GLES30.GL_STATIC_DRAW)

        val aPositionLocation = GLES30.glGetAttribLocation(program, "a_Position")
        GLES30.glVertexAttribPointer(aPositionLocation, 2, GLES30.GL_FLOAT, false, 4 * 4, 0)
        GLES30.glEnableVertexAttribArray(aPositionLocation)

        val aTexCoordLocation = GLES30.glGetAttribLocation(program, "a_TexCoord")
        GLES30.glVertexAttribPointer(aTexCoordLocation, 2, GLES30.GL_FLOAT, false, 4 * 4, 2 * 4)
        GLES30.glEnableVertexAttribArray(aTexCoordLocation)

        GLES30.glBindVertexArray(0)
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, 0)
    }

    fun draw(mvpMatrix: FloatArray, isPressed: Boolean) {
        GLES30.glUseProgram(program)

        val color = if (isPressed) {
            floatArrayOf(1f, 1f, 0f, 1f)
        } else {
            floatArrayOf(1f, 0f, 0f, 1f)
        }

        GLES30.glUniform4fv(uColorLocation, 1, color, 0)
        GLES30.glUniform1f(uRadiusLocation, 0.5f)
        GLES30.glUniform1f(uSmoothLocation, 0.01f)
        GLES30.glUniformMatrix4fv(uMvpMatrixLocation, 1, false, mvpMatrix, 0)

        GLES30.glBindVertexArray(vao[0])
        GLES30.glDrawArrays(GLES30.GL_TRIANGLE_STRIP, 0, 4)
        GLES30.glBindVertexArray(0)
    }

    fun release() {
        GLES30.glDeleteBuffers(1, vbo, 0)
        GLES30.glDeleteVertexArrays(1, vao, 0)
    }

    companion object {
        val SIZE = 2
    }
}